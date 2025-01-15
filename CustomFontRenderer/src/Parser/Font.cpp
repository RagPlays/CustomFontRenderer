#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <stdexcept>
#include <ctime>

#include <Engine.h>

#include "Font.h"

#define PARSELOGGING
#ifdef PARSELOGGING
#include <iostream>
#endif

using namespace Engine;

Font::Font(const std::string& filePath)
	: m_FontReader{ filePath }
	, m_FontHeader{}
{
	// Parsing //
	FontLog("\n");
	FontLog("## Font Parsing ------------------------------------------------------------------------");

	ReadFontHeader();
	ReadTableEntries();

	ReadHeadTable();
	ReadMaxpTable();
	ReadCmapTable();
	ReadLocaTable();
	ReadGlyfTable();

	FontLog("## Completed Parsing -------------------------------------------------------------------");
	FontLog("\n");

	m_FontReader.Close();
}

const std::vector<GlyphData>& Font::GetGlyphs() const
{
	return m_GlyphsData;
}

const GlyphData& Font::GetGlyphFromChar(char character) const
{
	ENGINE_ASSERT_MSG(!m_GlyphsData.empty(), "m_GlyphData must contain at least one glyph.");

	const auto it{ m_UnicodeToGlyphIdx.find(static_cast<uint32_t>(character)) };
	if (it != m_UnicodeToGlyphIdx.end()) return m_GlyphsData.at(it->second);
	return m_GlyphsData.at(0);
}

const std::string& Font::GetFontPath() const
{
	return m_FontReader.GetFontPath();
}

const uint16_t Font::GetUnitsPerEm() const
{
	return m_FontHeadTable.unitsPerEm;
}

uint32_t Font::GetTableOffset(const std::string& name) const
{
	const auto it{ m_TableEntries.find(name) };
	if (it != m_TableEntries.end())
	{
		return it->second.offset;
	}

	ENGINE_ERROR("Table entry not found: {0}", name);
	ENGINE_ASSERT(false);
	return 0;
}

uint32_t Font::GetGlyphLocFromIdx(uint32_t idx)
{
	ENGINE_ASSERT_MSG(!m_FontLocaTable.glyphOffsets.empty(), "Glyphs Data Needs To Be Loaded!");

	const uint32_t& glyphTableOffset{ GetTableOffset("glyf") };
	return m_FontLocaTable.glyphOffsets.at(idx) + glyphTableOffset;
}

void Font::ReadFontHeader()
{
	m_FontReader.ReadUInt32(&m_FontHeader.version);
	m_FontReader.ReadUInt16(&m_FontHeader.numTables);
	m_FontReader.ReadUInt16(&m_FontHeader.searchRange);
	m_FontReader.ReadUInt16(&m_FontHeader.entrySelector);
	m_FontReader.ReadUInt16(&m_FontHeader.rangeShift);

	PrintFontHeader();
}

void Font::ReadTableEntries()
{
	m_TableEntries.reserve(m_FontHeader.numTables);
	for (uint16_t tableDirIdx{}; tableDirIdx < m_FontHeader.numTables; ++tableDirIdx)
	{
		FontTableEntry tableData{};
		m_FontReader.ReadTag(&tableData.tag[0]);
		m_FontReader.ReadUInt32(&tableData.checkSum);
		m_FontReader.ReadUInt32(&tableData.offset);
		m_FontReader.ReadUInt32(&tableData.length);

		m_TableEntries[std::string(tableData.tag, 4)] = tableData;
	}

	PrintTableEntries();
}

void Font::ReadHeadTable()
{
	m_FontReader.SetPosition(GetTableOffset("head"));

	m_FontHeadTable.version = m_FontReader.ReadUInt32();
	m_FontHeadTable.fontRevision = m_FontReader.ReadUInt32();
	m_FontHeadTable.checkSumAdjustment = m_FontReader.ReadUInt32();
	m_FontHeadTable.magicNumber = m_FontReader.ReadUInt32();
	m_FontHeadTable.flags = m_FontReader.ReadUInt16();
	m_FontHeadTable.unitsPerEm = m_FontReader.ReadUInt16();
	m_FontHeadTable.created = m_FontReader.ReadUInt64();
	m_FontHeadTable.modified = m_FontReader.ReadUInt64();
	m_FontHeadTable.xMin = m_FontReader.ReadInt16();
	m_FontHeadTable.yMin = m_FontReader.ReadInt16();
	m_FontHeadTable.xMax = m_FontReader.ReadInt16();
	m_FontHeadTable.yMax = m_FontReader.ReadInt16();
	m_FontHeadTable.macStyle = m_FontReader.ReadUInt16();
	m_FontHeadTable.lowestRecPPEM = m_FontReader.ReadUInt16();
	m_FontHeadTable.fontDirectionHint = m_FontReader.ReadInt16();
	m_FontHeadTable.indexToLocFormat = m_FontReader.ReadInt16();
	m_FontHeadTable.glyphDataFormat = m_FontReader.ReadInt16();

	PrintHeadTable();
}

void Font::ReadMaxpTable()
{
	m_FontReader.SetPosition(GetTableOffset("maxp"));

	m_FontMaxpTable.version = m_FontReader.ReadUInt32();
	m_FontMaxpTable.numGlyphs = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxPoints = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxContours = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxCompositePoints = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxCompositeContours = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxZones = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxTwilightPoints = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxStorage = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxFunctionDefs = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxInstructionDefs = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxStackElements = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxSizeOfInstructions = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxComponentElements = m_FontReader.ReadUInt16();
	m_FontMaxpTable.maxComponentDepth = m_FontReader.ReadUInt16();

	PrintMaxpTable();
}

void Font::ReadCmapTable()
{
	const uint32_t& cmapTableLocation{ GetTableOffset("cmap") };
	m_FontReader.SetPosition(cmapTableLocation);

	const uint16_t version{ m_FontReader.ReadUInt16() };

	// can contain multiple character maps for different platforms
	const uint16_t numSubtables{ m_FontReader.ReadUInt16() };

	// --- Read through metadata for each character map to find the one we want to use ---
	uint32_t cmapSubtableOffset{ 0 };
	int selectedUnicodeVersionID{ -1 };

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

	ENGINE_ASSERT(cmapSubtableOffset != 0);

	m_FontReader.SetPosition(cmapTableLocation + static_cast<size_t>(cmapSubtableOffset));

	const uint16_t format{ m_FontReader.ReadUInt16() };
	bool hasMissingCharGlyph{ false };

	if (format != 12 && format != 4) // 4 and 12 are the main used formats
	{
		ENGINE_ERROR("Unsupported cmap format: {0}", format);
		ENGINE_ASSERT(false);
		return;
	}

	if (format == 12)
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
				const uint32_t uniCode{ startCharCode + charCodeOffset };
				const uint32_t glyphIndex{ startGlyphIndex + charCodeOffset };

				m_UnicodeToGlyphIdx.emplace(uniCode, glyphIndex);
			}
		}
	}
	else if (format == 4)
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
		ENGINE_ASSERT(reservedPad == 0); // always needs to be 0

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

		for (size_t codeIdx{}; codeIdx < startCodes.size(); ++codeIdx)
		{
			const uint16_t endCode{ endCodes[codeIdx] };
			uint32_t uniCode{ startCodes[codeIdx] };

			while (uniCode < endCode)
			{
				uint32_t glyphIndex{};

				// If idRangeOffset is 0, the glyph index can be calculated direclty
				if (idRangeOffsets[codeIdx].offset == 0)
				{
					glyphIndex = (uniCode + idDeltas[codeIdx]) % 65536;
				}
				// Otherwise, glyph index needs to be looked up from an array
				else
				{
					const size_t readerLocOld{ m_FontReader.GetPosition() };
					const uint32_t rangeOffsetLocation{ static_cast<uint32_t>(idRangeOffsets[codeIdx].readLoc + idRangeOffsets[codeIdx].offset) };
					const uint32_t glyphIdxArrayLocation{ static_cast<uint32_t>(2 * (uniCode - startCodes[codeIdx]) + rangeOffsetLocation) };

					m_FontReader.SetPosition(glyphIdxArrayLocation);
					glyphIndex = m_FontReader.ReadUInt16();

					if (glyphIndex != 0)
					{
						glyphIndex = (glyphIndex + idDeltas[codeIdx] % 65535);
					}

					m_FontReader.SetPosition(readerLocOld);
				}

				m_UnicodeToGlyphIdx.emplace(uniCode, glyphIndex);
				++uniCode;
			}
		}
	}

	FontLog("");
}

void Font::ReadLocaTable()
{
	constexpr uint8_t twoByteValue{ 2 };
	constexpr uint8_t fourByteValue{ 4 };

	// Use the preloaded data
	const uint16_t numGlyphs{ m_FontMaxpTable.numGlyphs };
	const uint16_t indexToLocFormat{ m_FontHeadTable.indexToLocFormat };
	
	// Checks
	if (indexToLocFormat != 0 && indexToLocFormat != 1)
	{
		ENGINE_ERROR("Unsupported index to loca format: {0}", indexToLocFormat);
		ENGINE_ASSERT(false);
		return;
	}

	const bool isTwoByteEntry{ indexToLocFormat == 0 };
	const uint8_t byteEntry{ isTwoByteEntry ? twoByteValue : fourByteValue };

	// Go To Table
	m_FontReader.SetPosition(GetTableOffset("loca"));

	// Reserve
	m_FontLocaTable.glyphOffsets.clear();
	m_FontLocaTable.glyphOffsets.reserve(numGlyphs);
	for (uint16_t glyphIndx{}; glyphIndx < numGlyphs; ++glyphIndx)
	{
		// If 2-byte format, the location is half from what it actually is (so times 2)
		const uint32_t glyphDataOffset{ isTwoByteEntry ? m_FontReader.ReadUInt16() * 2 : m_FontReader.ReadUInt32() };
		m_FontLocaTable.glyphOffsets.emplace_back(glyphDataOffset);
	}
}

void Font::ReadGlyfTable()
{
	m_GlyphsData.clear();
	m_GlyphsData.reserve(m_FontMaxpTable.numGlyphs);
	for (int idx{}; idx < m_FontMaxpTable.numGlyphs; ++idx)
	{
		m_GlyphsData.emplace_back(std::move(ReadGlyph(idx)));
	}
}

GlyphData Font::ReadGlyph(uint32_t glyphIdx)
{
	// Check if already exists
	if (glyphIdx < m_GlyphsData.size()) return m_GlyphsData[glyphIdx];

	// Glyph is either simple or compound
	// * Simple: outline data is stored here directly
	// * Compound: two or more simple glyphs need to be looked up, transformed, and combined

	m_FontReader.SetPosition(GetGlyphLocFromIdx(glyphIdx));
	return m_FontReader.ReadInt16() >= 0 ? ReadSimpleGlyph(glyphIdx) : ReadCompoundGlyph(glyphIdx);
}

GlyphData Font::ReadSimpleGlyph(uint32_t glyphIdx)
{
	constexpr uint8_t repeatBit{ 3 };
	GlyphData glyphData{};

	m_FontReader.SetPosition(GetGlyphLocFromIdx(glyphIdx));

	// The number of contours
	const int16_t contourCount{ m_FontReader.ReadInt16() };
	ENGINE_ASSERT(contourCount >= 0);

	// Bounds //
	m_FontReader.ReadInt16(&glyphData.xMin);
	m_FontReader.ReadInt16(&glyphData.yMin);
	m_FontReader.ReadInt16(&glyphData.xMax);
	m_FontReader.ReadInt16(&glyphData.yMax);

	// End of contour indices
	uint16_t numPoints{ 0 };
	glyphData.contourEndIndices.reserve(contourCount);
	for (uint16_t contourIdx{ 0 }; contourIdx < contourCount; ++contourIdx)
	{
		const uint16_t contourEndIdx{ m_FontReader.ReadUInt16() };
		numPoints = std::max(numPoints, static_cast<uint16_t>(contourEndIdx + 1));
		glyphData.contourEndIndices.emplace_back(contourEndIdx);
	}

	// Instructions
	const uint16_t instructionLength{ m_FontReader.ReadUInt16() };
	if (instructionLength > 0)
	{
		std::vector<uint8_t> instructions(instructionLength);
		m_FontReader.ReadBuffer(instructions.data(), instructionLength);
	}

	// Flags
	std::vector<uint8_t> allFlags(numPoints);
	for (int flagIdx{}; flagIdx < numPoints; ++flagIdx)
	{
		const uint8_t flag{ m_FontReader.ReadUInt8() };
		allFlags[flagIdx] = flag;

		if (ByteReader::IsBitSet(flag, repeatBit))
		{
			const uint8_t numOfCopies{ m_FontReader.ReadUInt8() };
			for (uint8_t flagCopyIdx{}; flagCopyIdx < numOfCopies; ++flagCopyIdx)
			{
				++flagIdx;
				allFlags[flagIdx] = flag;
			}
		}
	}

	// Points
	ReadGlyphPoints(glyphData.points, allFlags);

	return glyphData;
}

GlyphData Font::ReadCompoundGlyph(uint32_t glyphIdx)
{
	GlyphData glyphData{};

	m_FontReader.SetPosition(GetGlyphLocFromIdx(glyphIdx));

	const int16_t contourCount{ m_FontReader.ReadInt16() };
	ENGINE_ASSERT(contourCount < 0);

	// Bounds //
	m_FontReader.ReadInt16(&glyphData.xMin);
	m_FontReader.ReadInt16(&glyphData.yMin);
	m_FontReader.ReadInt16(&glyphData.xMax);
	m_FontReader.ReadInt16(&glyphData.yMax);

	while (true)
	{
		// Read In Glyph
		const auto [componentGlyph, moreGlyphs]{ ReadNextComponentGlyph() };
		const size_t contourEndIndicesOffset{ glyphData.contourEndIndices.size() };
		const size_t pointOffset{ glyphData.points.size() };

		// Reserve Space
		glyphData.contourEndIndices.reserve(contourEndIndicesOffset + componentGlyph.contourEndIndices.size());
		glyphData.points.reserve(pointOffset + componentGlyph.points.size());

		// Add Data To glyphData
		for (const uint16_t& endIndex : componentGlyph.contourEndIndices)
		{
			glyphData.contourEndIndices.emplace_back(static_cast<uint16_t>(endIndex + pointOffset));
		}
		glyphData.points.insert(glyphData.points.end(), componentGlyph.points.begin(), componentGlyph.points.end());

		// Check For More Glyphs
		if (!moreGlyphs) break;
	}

	return glyphData;
}

std::pair<GlyphData, bool> Font::ReadNextComponentGlyph()
{
	const uint16_t flags{ m_FontReader.ReadUInt16() };
	const uint16_t glyphIndex{ m_FontReader.ReadUInt16() };

	const bool argsAre2Bytes{ ByteReader::IsBitSet(flags, 0) };
	const bool argsAreXYValues{ ByteReader::IsBitSet(flags, 1) };
	const bool roundXYToGrid{ ByteReader::IsBitSet(flags, 2) };
	const bool weHaveAScale{ ByteReader::IsBitSet(flags, 3) };
	const bool moreComponents{ ByteReader::IsBitSet(flags, 5) };
	const bool weHaveAnXAndYScale{ ByteReader::IsBitSet(flags, 6) };
	const bool weHaveATwoByTwo{ ByteReader::IsBitSet(flags, 7) };
	const bool weHaveInstructions{ ByteReader::IsBitSet(flags, 8) };
	const bool useMyMetric{ ByteReader::IsBitSet(flags, 9) };
	const bool overlapCompound{ ByteReader::IsBitSet(flags, 10) };

	const double arg1{ static_cast<double>(argsAre2Bytes ? m_FontReader.ReadInt16() : m_FontReader.ReadInt8()) }; // X
	const double arg2{ static_cast<double>(argsAre2Bytes ? m_FontReader.ReadInt16() : m_FontReader.ReadInt8()) }; // Y

	double xOffset{};
	double yOffset{};
	double matchinPointInCompound{};
	double matchinPointInComponent{};
	if (argsAreXYValues)
	{
		xOffset = arg1;
		yOffset = arg2;
	}
	else
	{
		matchinPointInCompound = arg1;
		matchinPointInComponent = arg2;
	}

	double a{ 1.0 };
	double b{ 0.0 };
	double c{ 0.0 };
	double d{ 1.0 };

	if (weHaveAScale)
	{
		a = d = m_FontReader.ReadFixedPoint2Dot14();
	}
	else if (weHaveAnXAndYScale)
	{
		a = m_FontReader.ReadFixedPoint2Dot14();
		d = m_FontReader.ReadFixedPoint2Dot14();
	}
	else if (weHaveATwoByTwo)
	{
		a = m_FontReader.ReadFixedPoint2Dot14();
		b = m_FontReader.ReadFixedPoint2Dot14();
		c = m_FontReader.ReadFixedPoint2Dot14();
		d = m_FontReader.ReadFixedPoint2Dot14();
	}

	auto transformPoint = [&](double x, double y) -> std::pair<double, double>
		{
			const double xPrime{ (a * x + c * y) + xOffset };
			const double yPrime{ (b * x + d * y) + yOffset };
			return { xPrime, yPrime };
		};

	const size_t filePosition{ m_FontReader.GetPosition() };
	GlyphData glyphData{ ReadGlyph(glyphIndex) };
	m_FontReader.SetPosition(filePosition);
	for (auto& point : glyphData.points)
	{
		const auto [xPrime, yPrime] = transformPoint(point.x, point.y);
		if (roundXYToGrid)
		{
			point.x = static_cast<int>(std::round(xPrime));
			point.y = static_cast<int>(std::round(yPrime));
		}
		else
		{
			point.x = static_cast<int>(xPrime);
			point.y = static_cast<int>(yPrime);
		}
	}

	return { glyphData, moreComponents };
}

// ---------------- PRINTING ------------------- //
void Font::FontLog(const std::string& message)
{
#ifdef PARSELOGGING
	ENGINE_TRACE(message);
#endif
}

void Font::PrintFontHeader()
{
	FontLog("--- Font Header Info ---");
	FontLog("");
	FontLog("Font Version: " + std::to_string(m_FontHeader.version));
	FontLog("Number Of Tables: " + std::to_string(m_FontHeader.numTables));
	FontLog("Search Range: " + std::to_string(m_FontHeader.searchRange));
	FontLog("Entry Selector: " + std::to_string(m_FontHeader.entrySelector));
	FontLog("Range Shift: " + std::to_string(m_FontHeader.rangeShift));
	FontLog("");
}

void Font::PrintTableEntries()
{
	FontLog("--- Table Entries ---");
	FontLog("");
	for (const auto& [name, tableData] : m_TableEntries)
	{
		FontLog("Tag: " + name + "\t Location: " + std::to_string(tableData.offset));
	}
	FontLog("");
}

void Font::PrintHeadTable()
{
	FontLog("--- Font Head Table ---");
	FontLog("");
	FontLog("Version: " + std::to_string(m_FontHeadTable.version));
	FontLog("Font revision: " + std::to_string(m_FontHeadTable.fontRevision));
	FontLog("Check sum adjustment: " + std::to_string(m_FontHeadTable.checkSumAdjustment));
	FontLog("Magic number: " + std::to_string(m_FontHeadTable.magicNumber));
	FontLog("Flags: " + std::to_string(m_FontHeadTable.flags));
	FontLog("Units per EM: " + std::to_string(m_FontHeadTable.unitsPerEm));
	FontLog("Created (timestamp): " + std::to_string(m_FontHeadTable.created));
	FontLog("Modified (timestamp): " + std::to_string(m_FontHeadTable.modified));
	FontLog("Bounding box: [" + std::to_string(m_FontHeadTable.xMin) + ", " +
		std::to_string(m_FontHeadTable.yMin) + ", " +
		std::to_string(m_FontHeadTable.xMax) + ", " +
		std::to_string(m_FontHeadTable.yMax) + "]");
	FontLog("Mac style: " + std::to_string(m_FontHeadTable.macStyle));
	FontLog("Lowest rec PPEM: " + std::to_string(m_FontHeadTable.lowestRecPPEM));
	FontLog("Font direction hint: " + std::to_string(m_FontHeadTable.fontDirectionHint));
	FontLog("Index to loc format: " + std::to_string(m_FontHeadTable.indexToLocFormat));
	FontLog("Glyph data format: " + std::to_string(m_FontHeadTable.glyphDataFormat));
	FontLog("");
}

void Font::PrintMaxpTable()
{
	FontLog("--- Font Maxp Table ---");
	FontLog("");
	FontLog("Version: " + std::to_string(m_FontMaxpTable.version));
	FontLog("Number of Glyphs: " + std::to_string(m_FontMaxpTable.numGlyphs));
	FontLog("Max Points: " + std::to_string(m_FontMaxpTable.maxPoints));
	FontLog("Max Contours: " + std::to_string(m_FontMaxpTable.maxContours));
	FontLog("Max Composite Points: " + std::to_string(m_FontMaxpTable.maxCompositePoints));
	FontLog("Max Composite Contours: " + std::to_string(m_FontMaxpTable.maxCompositeContours));
	FontLog("Max Zones: " + std::to_string(m_FontMaxpTable.maxZones));
	FontLog("Max Twilight Points: " + std::to_string(m_FontMaxpTable.maxTwilightPoints));
	FontLog("Max Storage: " + std::to_string(m_FontMaxpTable.maxStorage));
	FontLog("Max Function Definitions: " + std::to_string(m_FontMaxpTable.maxFunctionDefs));
	FontLog("Max Instruction Definitions: " + std::to_string(m_FontMaxpTable.maxInstructionDefs));
	FontLog("Max Stack Elements: " + std::to_string(m_FontMaxpTable.maxStackElements));
	FontLog("Max Size of Instructions: " + std::to_string(m_FontMaxpTable.maxSizeOfInstructions));
	FontLog("Max Component Elements: " + std::to_string(m_FontMaxpTable.maxComponentElements));
	FontLog("Max Component Depth: " + std::to_string(m_FontMaxpTable.maxComponentDepth));
	FontLog("");
}

void Font::ReadGlyphPoints(std::vector<GlyphPoint>& points, const std::vector<uint8_t>& allFlags)
{
	constexpr uint8_t onCurveBit{ 0 };
	constexpr uint8_t isSingleByteBitX{ 1 };
	constexpr uint8_t isSingleByteBitY{ 2 };
	constexpr uint8_t instructionBitX{ 4 };
	constexpr uint8_t instructionBitY{ 5 };

	const size_t nrPoints{ allFlags.size() };
	points.resize(nrPoints);

	int prevX{ 0 };
	int prevY{ 0 };

	// Parse X
	for (size_t flagIdx = 0; flagIdx < nrPoints; ++flagIdx)
	{
		const uint8_t flag{ allFlags[flagIdx] };

		int x{ prevX };
		if (ByteReader::IsBitSet(flag, isSingleByteBitX))
		{
			const uint8_t offset{ m_FontReader.ReadUInt8() };
			const int8_t sign{ ByteReader::IsBitSet(flag, instructionBitX) ? 1 : -1 };
			x += static_cast<int>(offset * sign);
		}
		else if (!ByteReader::IsBitSet(flag, instructionBitX))
		{
			x += m_FontReader.ReadInt16();
		}

		points[flagIdx].x = x;
		prevX = x;
	}

	// Parse Y
	for (size_t flagIdx = 0; flagIdx < nrPoints; ++flagIdx)
	{
		const uint8_t flag{ allFlags[flagIdx] };

		int y{ prevY };
		if (ByteReader::IsBitSet(flag, isSingleByteBitY))
		{
			const uint8_t offset{ m_FontReader.ReadUInt8() };
			const int8_t sign{ ByteReader::IsBitSet(flag, instructionBitY) ? 1 : -1 };
			y += offset * sign;
		}
		else if (!ByteReader::IsBitSet(flag, instructionBitY))
		{
			y += m_FontReader.ReadInt16();
		}

		points[flagIdx].y = y;
		prevY = y;
	}

	// Get OnCurve
	for (size_t flagIdx = 0; flagIdx < nrPoints; ++flagIdx)
	{
		const uint8_t flag{ allFlags[flagIdx] };
		const bool onCurve{ ByteReader::IsBitSet(flag, onCurveBit) };
		points[flagIdx].onCurve = onCurve;
	}
}