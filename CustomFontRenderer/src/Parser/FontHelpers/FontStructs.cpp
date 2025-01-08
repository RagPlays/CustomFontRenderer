#include "FontStructs.h"

// ----------------- GLYPHPOINT -------------------- //
GlyphPoint::GlyphPoint(int x, int y)
    : x{ x }
    , y{ y }
    , onCurve{ false }
{
}

GlyphPoint::GlyphPoint(int x, int y, bool onCurve)
    : x{ x }
    , y{ y }
    , onCurve{ onCurve }
{
}

GlyphPoint::operator glm::ivec2() const
{
    return glm::ivec2{ x, y };
}

// ----------------- GLYPHDATA -------------------- //
uint16_t GlyphData::GetWidth() const
{
    return xMax - xMin;
}

uint16_t GlyphData::GetHeight() const
{
    return yMax - yMin;
}