#ifndef FONTSTRUCTS_H
#define FONTSTRUCTS_H

#include <cstdint>
#include <vector>
#include <string>

#include <Engine.h>
#include <glm/glm.hpp>

// ------------ TABLES DATA	 --------------- //
struct FontHeader
{
	uint32_t version;
	uint16_t numTables;
	uint16_t searchRange;
	uint16_t entrySelector;
	uint16_t rangeShift;
};

struct HeadTable
{
	uint32_t version;
	uint32_t fontRevision;
	uint32_t checkSumAdjustment;
	uint32_t magicNumber;
	uint16_t flags;
	uint16_t unitsPerEm;
	uint64_t created;
	uint64_t modified;
	int16_t xMin;
	int16_t yMin;
	int16_t xMax;
	int16_t yMax;
	uint16_t macStyle;
	uint16_t lowestRecPPEM;
	uint16_t fontDirectionHint;
	uint16_t indexToLocFormat;
	uint16_t glyphDataFormat;
};

struct MaxpTable
{
	uint32_t version;
	uint16_t numGlyphs;
	uint16_t maxPoints;
	uint16_t maxContours;
	uint16_t maxCompositePoints;
	uint16_t maxCompositeContours;
	uint16_t maxZones;
	uint16_t maxTwilightPoints;
	uint16_t maxStorage;
	uint16_t maxFunctionDefs;
	uint16_t maxInstructionDefs;
	uint16_t maxStackElements;
	uint16_t maxSizeOfInstructions;
	uint16_t maxComponentElements;
	uint16_t maxComponentDepth;
};

//struct CmapTable
//{
//	uint16_t version;
//	uint16_t numTables;
//
//
//
//};

struct LocaTable
{
	std::vector<uint32_t> glyphOffsets;
};

struct FontTableEntry
{
	char tag[4];
	uint32_t checkSum;
	uint32_t offset;
	uint32_t length;
};

// ------------ GLYPH DATA	 --------------- //
struct GlyphPoint
{
	int x;
	int y;
	bool onCurve;

	GlyphPoint() = default;
	explicit GlyphPoint(int x, int y);
	explicit GlyphPoint(int x, int y, bool onCurve);

	operator glm::ivec2() const;
};

struct GlyphData
{
	int16_t xMin;
	int16_t xMax;
	int16_t yMin;
	int16_t yMax;

	uint32_t unicodeValue;
	uint32_t glyphIndex;
	std::vector<GlyphPoint> points;
	std::vector<uint16_t> contourEndIndices;
	uint32_t advanceWidth;
	uint32_t leftSideBearing;

	uint16_t GetWidth() const;
	uint16_t GetHeight() const;
};

struct GlyphMap
{
	uint32_t glyphIdx;
	uint32_t unicode;
};

struct GlyphBounds
{
	glm::vec2 center{};
	glm::vec2 size{};
};

// ------------ TEXT DATA	 --------------- //
//struct TextData
//{
//
//};

#endif // !FONTSTRUCTS_H