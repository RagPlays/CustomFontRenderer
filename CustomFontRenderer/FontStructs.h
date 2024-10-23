#ifndef FONTSTRUCTS_H
#define FONTSTRUCTS_H

#include <vector>
#include <string>

#include "Structs.h"

struct GlyphData
{
	GlyphData() = default;
	explicit GlyphData(int16_t xMin, int16_t xMax, int16_t yMin, int16_t yMax, const std::vector<uint16_t>& contourEndIndices, const std::vector<Point2i>& points);

	int16_t xMin;
	int16_t xMax;
	int16_t yMin;
	int16_t yMax;

	std::vector<uint16_t> contourEndIndices;
	std::vector<Point2i> points;
};

struct TableData
{
	TableData() = default;
	explicit TableData(const std::string& tag, const uint32_t& checkSum, const uint32_t& offset, const uint32_t& length);

	std::string tag;
	uint32_t checkSum;
	uint32_t offset;
	uint32_t length;
};

#endif // !FONTSTRUCTS_H