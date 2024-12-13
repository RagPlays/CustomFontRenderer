#include "FontRenderer.h"

#include "Parser/Font.h"

using namespace Engine;

std::vector<glm::vec4> FontRenderer::s_ContourLineColors
{
	{ 1.f, 0.16f, 0.16f, 1.f },
	{ 0.16f, 1.f, 0.16f, 1.f },
	{ 0.16f, 0.16f, 1.f, 1.f }
};

FontRenderer::FontRenderer(std::shared_ptr<Font> font)
	: m_Font{ font }
	, m_RenderConfigs{}
{
}

FontRenderer::~FontRenderer()
{
}

void FontRenderer::SetRenderConfigs(const FontRenderConfigs& configs)
{
	m_RenderConfigs = configs;
}

void FontRenderer::Render() const
{
	constexpr int wordSpacing{ 400 };
	constexpr int letterSpacing{ 700 };

	glm::ivec2 offset{};
	for (const char& textChar : m_RenderConfigs.inputText)
	{
		if (textChar == ' ')
		{
			offset.x += wordSpacing;
			continue;
		}

		const GlyphData& charGlyph{ m_Font->GetGlyphFromChar(textChar) };
		RenderGlyph(charGlyph, offset);

		offset.x += letterSpacing;
	}
}

void FontRenderer::RenderGlyph(const GlyphData& glyph, const glm::ivec2& offset) const
{
	const std::vector<uint16_t>& contourEndIndices{ glyph.contourEndIndices };
	const std::vector<GlyphPoint>& points{ glyph.points };
	const int glyphWidth{ glyph.xMax - glyph.xMin };
	const int glyphHeight{ glyph.yMax - glyph.yMin };
	const Engine::Rect2i glyphbox{ glm::ivec2{ glyph.xMin + glyphWidth / 2, glyph.yMin + glyphHeight / 2 } + offset, { glyphWidth, glyphHeight } };

	// Glyphbox
	if (m_RenderConfigs.drawDebugBoundBox)
	{
		Renderer2D::SetDrawColor(Color::white);
		Renderer2D::DrawRect(static_cast<Rect2f>(glyphbox));
	}
	
	// Lines
	if (m_RenderConfigs.drawDebugLines)
	{
		Renderer2D::SetLineWidth(1.f);
		int contourStartIdx{};
		for (size_t contourEndIdx{}; contourEndIdx < contourEndIndices.size(); ++contourEndIdx)
		{
			Renderer2D::SetDrawColor(s_ContourLineColors[contourEndIdx % s_ContourLineColors.size()]);

			const uint16_t& currentEndIdx{ contourEndIndices[contourEndIdx] };

			const size_t numPontsInContour{ static_cast<size_t>(currentEndIdx - contourStartIdx + 1) };
			const std::vector<GlyphPoint> contourPoints
			{
				points.begin() + contourStartIdx,
				points.begin() + contourStartIdx + numPontsInContour
			};

			for (size_t pointIdx{}; pointIdx < numPontsInContour; ++pointIdx)
			{
				const glm::ivec2& pointOne{ static_cast<glm::ivec2>(contourPoints[pointIdx]) + offset };
				const glm::ivec2& pointTwo{ static_cast<glm::ivec2>(contourPoints[(pointIdx + 1) % numPontsInContour]) + offset };
				Renderer2D::DrawLine(static_cast<glm::vec2>(pointOne), static_cast<glm::vec2>(pointTwo));
			}

			contourStartIdx = currentEndIdx + 1;
		}
	}

	// Points
	if (m_RenderConfigs.drawDebugPoints)
	{
		Renderer2D::SetPointSize(2.f);
		Renderer2D::SetDrawColor(Color::white);
		for (const GlyphPoint& point : points)
		{
			const glm::ivec2 drawPos{ glm::ivec2{ point.x, point.y } + offset };
			Renderer2D::DrawPoint(drawPos);
		}
	}
}