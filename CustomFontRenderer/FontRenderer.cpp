#include "FontRenderer.h"

#include <cstdint>
#include <string>
#include <vector>
#include <SDL2/SDL_opengl.h>

#include "Structs.h"
#include "FontParser.h"
#include "DrawUtils.h"
#include "CollisionUtils.h"
#include "Camera.h"
#include "FontStructs.h"

static void GetViewportDimensions(int& width, int& height)
{
	// Get the viewport dimensions
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	width = viewport[2];  // width
	height = viewport[3]; // height
}

std::vector<Color4f> FontRenderer::s_ContourLineColors
{
	Color4f{ 255, 40, 40 },
	Color4f{ 40, 255, 40 },
	Color4f{ 40, 40, 255 }
};

FontRenderer::FontRenderer(FontParser* parser, const std::string& text)
	: m_pFontParser{ parser }
	, m_Text{ text }
{
	LoadTextGlyphs();
}

void FontRenderer::Render() const
{
	constexpr int wordSpacing{ 400 };
	constexpr int letterSpacing{ 700 };

	Point2i offset{};
	for (int charIdx{}; charIdx < m_TextGlyphs.size(); ++charIdx)
	{
		const char& textChar{ m_Text[charIdx] };
		if (textChar == ' ')
		{
			offset.x += wordSpacing;
			continue;
		}

		DrawGlyph(m_TextGlyphs[charIdx], offset);
		offset.x += letterSpacing;
	}

	/*const std::vector<GlyphData>& glyphs{ m_pFontParser->GetGlyphsData() };
	Point2i offset{};
	for (const auto& glyph : glyphs)
	{
		DrawGlyph(glyph, offset);
		offset.x += letterSpacing;
	}*/
}

void FontRenderer::LoadTextGlyphs()
{
	m_TextGlyphs.clear();
	m_TextGlyphs.reserve(m_Text.size());

	for (char textChar : m_Text)
	{
		const GlyphData& glyphData{ m_pFontParser->GetGlyphFromChar(textChar) };
		m_TextGlyphs.emplace_back(glyphData);
	}
}

void FontRenderer::DrawGlyph(const GlyphData& glyphData, const Point2i& offset) const
{
	const std::vector<uint16_t>& contourEndIndices{ glyphData.contourEndIndices };
	const std::vector<Point2i>& points{ glyphData.points };

	//const Point2f& camPos{ Camera::Get().GetPosition() };
	//int width, height;
	//GetViewportDimensions(width, height);
	//const Recti cameraRect{ static_cast<Point2i>(camPos), width, height };

	const Recti glyphBox{ Point2i{ glyphData.xMin, glyphData.yMin }, glyphData.xMax - glyphData.xMin, glyphData.yMax - glyphData.yMin };

	//if (!CollisionUtils::IsOverlapping(cameraRect, Recti{ Point2i{glyphBox.x, glyphBox.y} + offset, glyphBox.width, glyphBox.height })) return;
	
	// Draw Lines
	DrawUtils::SetLineThickness(2.f);
	int contourStartIdx{};
	for (size_t contourEndIdx{}; contourEndIdx < contourEndIndices.size(); ++contourEndIdx)
	{
		DrawUtils::SetColor(s_ContourLineColors[contourEndIdx % s_ContourLineColors.size()]);

		const uint16_t& currentEndIdx{ contourEndIndices[contourEndIdx] };

		const size_t numPointsInContour{ static_cast<size_t>(currentEndIdx - contourStartIdx + 1) };
		const std::vector<Point2i> contourPoints(points.begin() + contourStartIdx, points.begin() + contourStartIdx + numPointsInContour);

		for (size_t pointIdx{}; pointIdx < numPointsInContour; ++pointIdx)
		{
			const Point2i& point1{ contourPoints[pointIdx] };
			const Point2i& point2{ contourPoints[(pointIdx + 1) % numPointsInContour] };
			DrawUtils::Line(point1, point2);
		}

		contourStartIdx = currentEndIdx + 1;
	}

	// Draw Points
	DrawUtils::SetPointSize(5.f);
	DrawUtils::SetColor(Color4f::white);
	DrawUtils::Points(points);

	// Draw GlyphBox
	DrawUtils::Rectangle(glyphBox);

}