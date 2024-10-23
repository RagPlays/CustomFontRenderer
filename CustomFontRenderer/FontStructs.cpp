#include "FontStructs.h"

// Glyph Data //

GlyphData::GlyphData(int16_t xMin, int16_t xMax, int16_t yMin, int16_t yMax, const std::vector<uint16_t>& contourEndIndices, const std::vector<Point2i>& points)
	: xMin{ xMin }
	, xMax{ xMax }
	, yMin{ yMin }
	, yMax{ yMax }
	, contourEndIndices{ contourEndIndices }
	, points{ points }
{
}

// Table Data //

TableData::TableData(const std::string& tag, const uint32_t& checkSum, const uint32_t& offset, const uint32_t& length)
	: tag{ tag }
	, checkSum{ checkSum }
	, offset{ offset }
	, length{ length }
{
}