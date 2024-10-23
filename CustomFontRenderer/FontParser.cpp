#include <cassert>

#include "FontParser.h"

#if defined DEBUG || defined _DEBUG
#define PARSELOGGING
	#ifdef PARSELOGGING
		#include <iostream>
		#define DEFAULTDATALOGGING
		//#define GLYPHLOCATIONLOGGING
		//#define FLAGLOGGING
		#ifdef FLAGLOGGING
			#define DETAILEDFLAGLOGGING
		#endif
		//#define GLYPHDATALOGGING
		#ifdef GLYPHDATALOGGING
			#define INSTRUCTIONLOGGING
			#define ENDOFCOUNTOURLOGGING
			#define COORDINATESLOGGING
		#endif
	#endif
#endif

FontParser::FontParser(const std::string& filePath)
	: m_FontReader{ filePath }
{
	ReadTableLocations();
	ReadGlyphLocations();
	ReadGlyphs();
	ReadUniCodeToIndex();
}

const GlyphData& FontParser::GetGlyphFromChar(char character) const
{
	const uint32_t charuint32{ static_cast<uint32_t>(character) };
	const auto it{ m_GlyphUnicodeToIndexLookup.find(charuint32) };
	if (it != m_GlyphUnicodeToIndexLookup.end())
	{
		const GlyphData& glyph{ m_GlyphsData.at(it->second) };
		if (!glyph.points.empty())
		{
			return glyph;
		}
	}

	return m_GlyphsData.at(0);
}

const std::vector<GlyphData>& FontParser::GetGlyphsData() const
{
	return m_GlyphsData;
}

void FontParser::ReadTableLocations()
{
	// Default Values //
	// Scaler type
	const uint32_t scalerType{ m_FontReader.ReadUInt32() };
	// Number of tables
	const uint16_t numTables{ m_FontReader.ReadUInt16() };
	// Search range
	const uint16_t searchRange{ m_FontReader.ReadUInt16() };
	// Entry selector
	const uint16_t entrySelector{ m_FontReader.ReadUInt16() };
	// Range shift
	const uint16_t rangeShift{ m_FontReader.ReadUInt16() };

	// Table Directory //
	m_TableLocationsLookup.reserve(numTables);
	for (uint16_t tableDirIdx{}; tableDirIdx < numTables; ++tableDirIdx)
	{
		const std::string tag{ m_FontReader.ReadTag() };
		const uint32_t checkSum{ m_FontReader.ReadUInt32() };
		const uint32_t offset{ m_FontReader.ReadUInt32() };
		const uint32_t length{ m_FontReader.ReadUInt32() };
		m_TableLocationsLookup[tag] = TableData{ tag, checkSum, offset, length };
	}

#ifdef DEFAULTDATALOGGING
	ParseLog("\n|Default Values|\n");
	ParseLog("Scaler type: " + std::to_string(scalerType) + " (32bit)\n");
	ParseLog("NumTables: " + std::to_string(numTables) + " (16bit)\n");
	ParseLog("SearchRange: " + std::to_string(searchRange) + " (16bit)\n");
	ParseLog("EntrySelector: " + std::to_string(entrySelector) + " (16bit)\n");
	ParseLog("RangeShift: " + std::to_string(rangeShift) + " (16bit)\n");
	ParseLog("\n|Table Directory|\n");
	for (const auto& tablePair : m_TableLocationsLookup)
	{
		ParseLog("Tag: " + tablePair.first + " Location: " + std::to_string(tablePair.second.offset) + "\n");
	}
#endif
}

void FontParser::ReadGlyphLocations()
{
	constexpr uint8_t twoByteValue{ 2 };
	constexpr uint8_t fourByteValue{ 4 };

	// number of glyps is in the 'maxp' table
	const uint32_t& maxpTableLocation{ m_TableLocationsLookup["maxp"].offset };
	m_FontReader.SetPosition(maxpTableLocation);
	m_FontReader.SkipBytes(4); // skip version

	const uint16_t numGlyphs{ m_FontReader.ReadUInt16() };

	// Get format of location indeices from 'head' table
	const uint32_t& headTableLocation{ m_TableLocationsLookup["head"].offset };
	m_FontReader.SetPosition(headTableLocation);
	// Skip unused: version, fontRevision, checkSumAdjustment, magicNumber, flags,
	// design units, dates, max bounds, macStyles, lowestRecPPEM, directionHintFlag
	m_FontReader.SkipBytes(50);

	// Get indexToLocFormat: 0 or 1, indicating 2 or 4-byte format for location lookup
	const bool isTwoByteEntry{ m_FontReader.ReadInt16() == 0 };
	const uint8_t byteEntry{ isTwoByteEntry ? twoByteValue : fourByteValue };

	// Get glyph locations from 'loca' table
	const uint32_t& locaTableLocation{ m_TableLocationsLookup["loca"].offset };
	const uint32_t& glyphTableLocation{ m_TableLocationsLookup["glyf"].offset };

	m_GlyphsData.reserve(numGlyphs);
	m_GlyphLocations.resize(numGlyphs);
	for (uint16_t glyphIndx{}; glyphIndx < numGlyphs; ++glyphIndx)
	{
		const uint32_t location{ locaTableLocation + glyphIndx * byteEntry };
		m_FontReader.SetPosition(location);

		// If 2-byte format, the location is half from what it actually is (so times 2)
		const uint32_t glyphDataOffset{ isTwoByteEntry ? m_FontReader.ReadUInt16() * 2 : m_FontReader.ReadUInt32() };
		m_GlyphLocations[glyphIndx] = glyphTableLocation + glyphDataOffset;
	}

#ifdef GLYPHLOCATIONLOGGING
	for (size_t glyphLocationIdx{}; glyphLocationIdx < m_GlyphLocations.size(); ++glyphLocationIdx)
	{
		ParseLog("Glyph location " + std::to_string(glyphLocationIdx) + ": " + std::to_string(m_GlyphLocations[glyphLocationIdx]) + "\n");
	}
#endif
}

void FontParser::ReadGlyphs()
{
	for (const uint32_t& glyphLocation : m_GlyphLocations)
	{
		const GlyphData glyph{ ReadGlyph(glyphLocation) };
		m_GlyphsData.emplace_back(glyph);
	}
}

void FontParser::ReadUniCodeToIndex()
{
	const uint32_t& cmapTableLocation{ m_TableLocationsLookup["cmap"].offset };
	m_FontReader.SetPosition(cmapTableLocation);

	const uint16_t version{ m_FontReader.ReadUInt16() };

	// can contain multiple character maps for different platforms
	const uint16_t numSubtables{ m_FontReader.ReadUInt16() };

	// --- Read through metadata for each character map to find the one we want to use ---
	uint32_t cmapSubtableOffset = 0;
	int selectedUnicodeVersionID = -1;

	for (int subTableIdx{}; subTableIdx < numSubtables; ++subTableIdx)
	{
		const int platformID{ m_FontReader.ReadUInt16() };
		const int platformSpecificID{ m_FontReader.ReadUInt16() };
		const uint32_t offset{ m_FontReader.ReadUInt32() };

		// Unicode encoding
		if (platformID == 0)
		{
			// Use highest supported unicode version
			if ((platformSpecificID == 0 || platformSpecificID == 1 ||
				platformSpecificID == 3 || platformSpecificID == 4) &&
				platformSpecificID > selectedUnicodeVersionID)
			{
				cmapSubtableOffset = offset;
				selectedUnicodeVersionID = platformSpecificID;
			}
		}
		// Microsoft Encoding
		else if (platformID == 3 && selectedUnicodeVersionID == -1)
		{
			if (platformSpecificID == 1 || platformSpecificID == 10)
			{
				cmapSubtableOffset = offset;
			}
		}
	}

	assert(cmapSubtableOffset != 0);

	m_FontReader.SetPosition(cmapTableLocation + cmapSubtableOffset);

	const uint16_t format{ m_FontReader.ReadUInt16() };
	bool hasMissingCharGlyph{ false };

	if (format != 12 && format != 4) // 4 and 12 are the main used formats
	{
		assert(false);
	}

	if (format == 4)
	{
		const uint16_t subtableLength{ m_FontReader.ReadUInt16() };
		const uint16_t language{ m_FontReader.ReadUInt16() };
		const uint16_t segCountX2{ m_FontReader.ReadUInt16() };
		const uint16_t segCount{ static_cast<uint16_t>(segCountX2 / 2) };

		const uint16_t searchRange{ m_FontReader.ReadUInt16() };
		const uint16_t entrySelector{ m_FontReader.ReadUInt16() };
		const uint16_t rangeShift{ m_FontReader.ReadUInt16() };

		// for loop for endCode's
		std::vector<uint16_t> endCodes{};
		endCodes.resize(segCount);
		for (uint16_t& endCode : endCodes)
		{
			endCode = m_FontReader.ReadUInt16();
		}

		const uint16_t reservedPad{ m_FontReader.ReadUInt16() };
		assert(reservedPad == 0); // always needs to be 0

		// for loop for startCode's
		std::vector<uint16_t> startCodes{};
		startCodes.resize(segCount);
		for (uint16_t& startCode : startCodes)
		{
			startCode = m_FontReader.ReadUInt16();
		}

		// for loop for idDelta's
		std::vector<uint16_t> idDeltas{};
		idDeltas.resize(segCount);
		for (uint16_t& idDelta : idDeltas)
		{
			idDelta = m_FontReader.ReadUInt16();
		}

		// for loop for idRangeOffset's
		struct IdRange
		{
			size_t readLoc;
			uint16_t offset;
		};
		std::vector<IdRange> idRangeOffsets{};
		idRangeOffsets.resize(segCount);
		for (IdRange& idRangeOffset : idRangeOffsets)
		{
			const size_t readLoc{ m_FontReader.GetPosition() };
			const uint16_t offset{ m_FontReader.ReadUInt16() };

			idRangeOffset = IdRange{ readLoc, offset };
		}

		for (int codeIdx{}; codeIdx < startCodes.size(); ++codeIdx)
		{
		}

	}
	else if (format == 12)
	{
		const uint16_t reserved{ m_FontReader.ReadUInt16() };
		const uint32_t length{ m_FontReader.ReadUInt32() };
		const uint32_t language{ m_FontReader.ReadUInt32() };
		const uint32_t numGroups{ m_FontReader.ReadUInt32() };

		for (uint32_t groupIdx{}; groupIdx < numGroups; groupIdx++)
		{
			const uint32_t startCharCode{ m_FontReader.ReadUInt32() };
			const uint32_t endCharCode{ m_FontReader.ReadUInt32() };
			const uint32_t startGlyphIndex{ m_FontReader.ReadUInt32() };

			const uint32_t numChars{ static_cast<uint32_t>(endCharCode - startCharCode + 1) };
			for (uint32_t charCodeOffset{}; charCodeOffset < numChars; charCodeOffset++)
			{
				const uint32_t charCode{ startCharCode + charCodeOffset };
				const uint32_t glyphIndex{ startGlyphIndex + charCodeOffset };

				m_GlyphUnicodeToIndexLookup[charCode] = glyphIndex;
				//m_GlyphUnicodeToIndexLookup.insert(charCode, glyphIndex);
				
				//hasReadMissingCharGlyph |= glyphIndex == 0;
			}
		}
	}
}

GlyphData FontParser::ReadGlyph(uint32_t glyphPos)
{
	m_FontReader.SetPosition(static_cast<size_t>(glyphPos));

	const int16_t contourCount{ m_FontReader.ReadInt16() };

	// Glyph is either simple or compound
	// * Simple: outline data is stored here directly
	// * Compound: two or more simple glyphs need to be looked up, transformed, and combined
	const bool isSimpleGlyph{ contourCount >= 0 };

	return isSimpleGlyph ? ReadSimpleGlyph(glyphPos) : ReadCompoundGlyph(glyphPos);
}

GlyphData FontParser::ReadSimpleGlyph(uint32_t glyphPos)
{
	GlyphData glyphData{};

	constexpr int repeatBit = 3;

	m_FontReader.SetPosition(static_cast<size_t>(glyphPos));

	// The number of contours
	const int16_t contourCount{ m_FontReader.ReadInt16() };
	assert(contourCount >= 0);

	// Minimum x for coordinate data
	glyphData.xMin = m_FontReader.ReadInt16();
	// Minimum y for coordinate data
	glyphData.yMin = m_FontReader.ReadInt16();
	// Maximum x for coordinate data
	glyphData.xMax = m_FontReader.ReadInt16();
	// Maximum y for coordinate data
	glyphData.yMax = m_FontReader.ReadInt16();

	// End of contour indices
	uint16_t numPoints{};
	glyphData.contourEndIndices.reserve(contourCount);
	for (uint16_t contourIdx{}; contourIdx < contourCount; ++contourIdx)
	{
		const uint16_t contourEndIdx{ m_FontReader.ReadUInt16() };
		numPoints = std::max(numPoints, static_cast<uint16_t>(contourEndIdx + 1));
		glyphData.contourEndIndices.emplace_back(contourEndIdx);
	}

	// Instructions
	const uint16_t instructionLength{ m_FontReader.ReadUInt16() };
	const std::vector<uint8_t> instructions{ m_FontReader.ReadBytes(static_cast<size_t>(instructionLength)) };

	// Flags
	std::vector<uint8_t> allFlags{};
	allFlags.resize(numPoints);

	for (int flagIdx{}; flagIdx < numPoints; ++flagIdx)
	{
		const uint8_t flag{ m_FontReader.ReadUint8() };
		allFlags[flagIdx] = flag;

		if (ByteReader::IsBitSet(flag, repeatBit))
		{
			const uint8_t numOfCopies{ m_FontReader.ReadUint8() };
			for (uint8_t flagCopyIdx{}; flagCopyIdx < numOfCopies; ++flagCopyIdx)
			{
				++flagIdx;
				allFlags[flagIdx] = flag;
			}
		}
	}

	// Points
	const auto xCoordinates{ ParseCoordinates(allFlags, true) };
	const auto yCoordinates{ ParseCoordinates(allFlags, false) };

	glyphData.points.reserve(numPoints);
	for (size_t pointIdx{}; pointIdx < numPoints; ++pointIdx)
	{
		const int& xCoord{ xCoordinates[pointIdx] };
		const int& yCoord{ yCoordinates[pointIdx] };
		glyphData.points.emplace_back(Point2i{ xCoord, yCoord });
	}

#ifdef GLYPHDATALOGGING
	// Loggin Everything //
	ParseLog("\n|glyf Data|\n");
	ParseLog("\n");
	ParseLog("-glyph description-\n");
	ParseLog("Number of contours: " + std::to_string(contourCount) + "\n");
	ParseLog("Minimum x for coordinate data: " + std::to_string(glyphData.xMin) + "\n");
	ParseLog("Minimum y for coordinate data: " + std::to_string(glyphData.yMin) + "\n");
	ParseLog("Maximum x for coordinate data: " + std::to_string(glyphData.xMax) + "\n");
	ParseLog("Maximum y for coordinate data: " + std::to_string(glyphData.yMax) + "\n");
	ParseLog("\n-End points indices of contours-\n");
#ifdef ENDOFCOUNTOURLOGGING
	for (size_t contourIdx{}; contourIdx < contourCount; ++contourIdx)
	{
		ParseLog("Contour end index " + std::to_string(contourIdx) + ": " + std::to_string(glyphData.contourEndIndices[contourIdx]) + "\n");
	}
#endif
	ParseLog("\n");
	ParseLog("-Instructions-\n");
	ParseLog("Instruction length: " + std::to_string(instructionLength) + "\n");
#ifdef INSTRUCTIONLOGGING
	for (size_t instructionIdx{}; instructionIdx < instructions.size(); ++instructionIdx)
	{
		ParseLog("Instruction " + std::to_string(instructionIdx) + ": " + std::to_string(instructions[instructionIdx]) + "\n");
	}
#endif
	ParseLog("\n");
	ParseLog("-Flags-\n");
	ParseLog("Number of points: " + std::to_string(numPoints) + "\n");

	// Log them
	for (size_t flagIdx{}; flagIdx < allFlags.size(); ++flagIdx)
	{
		const uint8_t& flag{ allFlags[flagIdx] };
#ifdef DETAILEDFLAGLOGGING
		ParseLogFlag(flagIdx, flag);
#else
		ParseLog("Flag " + std::to_string(flagIdx) + ": " + std::to_string(flag) + "\n");
#endif
	}
	ParseLog("\n");
	ParseLog("-Coordinates-\n");
	for (size_t pointIdx{}; pointIdx < numPoints; ++pointIdx)
	{
		const int& xCoord{ glyphData.points[pointIdx].x };
		const int& yCoord{ glyphData.points[pointIdx].y };
#ifdef COORDINATESLOGGING
		ParseLog("Point " + std::to_string(pointIdx) + ": (" + std::to_string(xCoord) + ", " + std::to_string(yCoord) + ")\n");
#endif
	}
#endif

	return glyphData;
}

GlyphData FontParser::ReadCompoundGlyph(uint32_t glyphPos)
{
	return m_GlyphsData.at(0); // for now
}

std::vector<int> FontParser::ParseCoordinates(const std::vector<uint8_t>& allFlags, bool readingX)
{
	constexpr int onCurveBit = 0;
	constexpr int isSingleByteXBit = 1;
	constexpr int isSingleByteYBit = 2;
	constexpr int instructionXBit = 4;
	constexpr int instructionYBit = 5;

	const int singelByteFlagBit = readingX ? isSingleByteXBit : isSingleByteYBit;
	const int instructionFlagBit = readingX ? instructionXBit : instructionYBit;

	std::vector<int> coordinates{};
	coordinates.resize(allFlags.size());

	for (size_t idx{}; idx < coordinates.size(); ++idx)
	{
		const int& previousCoord = coordinates[std::max(0, static_cast<int>(idx - 1))];
		coordinates[idx] = previousCoord;

		const uint8_t& flag = allFlags[idx];
		const bool onCurve{ ByteReader::IsBitSet(flag, onCurveBit) };

		if (ByteReader::IsBitSet(flag, singelByteFlagBit))
		{
			const uint8_t offset{ m_FontReader.ReadUint8() };
			const int sign = ByteReader::IsBitSet(flag, instructionFlagBit) ? 1 : -1;
			coordinates[idx] += offset * sign;
		}
		else if (!ByteReader::IsBitSet(flag, instructionFlagBit))
		{
			coordinates[idx] += m_FontReader.ReadInt16();
		}
	}

	return coordinates;
}

void FontParser::ParseLog(const std::string& message)
{
#ifdef PARSELOGGING
	std::cout << message;
#endif
}

void FontParser::ParseLogFlag(size_t flagIdx, uint8_t flag)
{
	ParseLog("** Flag " + std::to_string(flagIdx) + " **\n");

	const bool onTheCurve{ ByteReader::IsBitSet(flag, 0) };
	ParseLog("On the curve: " + std::to_string(onTheCurve) + "\n");

	const bool oneByteLongX{ ByteReader::IsBitSet(flag, 1) };
	ParseLog("One byte long X: " + std::to_string(oneByteLongX) + "\n");

	const bool oneByteLongY{ ByteReader::IsBitSet(flag, 2) };
	ParseLog("One byte long Y: " + std::to_string(oneByteLongY) + "\n");

	const bool repeatFlag{ ByteReader::IsBitSet(flag, 3) };
	ParseLog("Repeat flag: " + std::to_string(repeatFlag) + "\n");

	const bool posShortVecX{ ByteReader::IsBitSet(flag, 4) };
	ParseLog("Positive short vec X:	" + std::to_string(posShortVecX) + "\n");

	const bool posShortVecY{ ByteReader::IsBitSet(flag, 5) };
	ParseLog("Positive short vec Y:	" + std::to_string(posShortVecY) + "\n");

	// bit 6 and 7 (reserved, always 0)
}