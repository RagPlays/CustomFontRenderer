#include "FontRenderer.h"

#include "Parser/Font.h"
#include "Parser/FontHelpers/FontData.h"
#include "Parser/FontHelpers/GlyphHelpers.h"

using namespace Engine;

const std::vector<glm::vec4> FontRenderer::s_ContourLineColors
{
	{ 1.f, 0.16f, 0.16f, 1.f },
	{ 0.16f, 1.f, 0.16f, 1.f },
	{ 0.16f, 0.16f, 1.f, 1.f }
};

FontRenderer::FontRenderer()
	: m_Font{ nullptr }
	, m_RenderConfigs{}
	, m_TextGlyphs{}
{
}

FontRenderer::FontRenderer(const FontRenderConfigs& configs)
	: m_Font{ nullptr }
	, m_RenderConfigs{}
	, m_TextGlyphs{}
{
	UpdateRenderData(m_RenderConfigs, true, true);
}

FontRenderer::FontRenderer(std::shared_ptr<Font> font)
	: m_Font{ font }
	, m_RenderConfigs{}
	, m_TextGlyphs{}
{
	UpdateRenderData(m_RenderConfigs, true, false);
}

FontRenderer::~FontRenderer()
{
}

void FontRenderer::SetRenderConfigs(const FontRenderConfigs& newConfigs)
{
	if (m_RenderConfigs != newConfigs)
	{
		UpdateRenderData(newConfigs);
		m_RenderConfigs = newConfigs;
	}
}

void FontRenderer::UpdateRenderData(const FontRenderConfigs& newConfigs, bool forceTextReload, bool forceFontReload)
{
	const bool fontChanged{ m_RenderConfigs.fontPath != newConfigs.fontPath };
	const bool textChanged{ m_RenderConfigs.inputText != newConfigs.inputText };

	if (fontChanged || forceFontReload)
	{
		m_Font = std::make_shared<Font>(newConfigs.fontPath);
	}

	if (fontChanged || textChanged || forceTextReload)
	{
		m_TextGlyphs.clear();
		m_TextGlyphs.reserve(newConfigs.inputText.size());

		for (const char& textChar : newConfigs.inputText)
		{
			if (textChar == ' ' || textChar == '\n' || textChar == '\t') continue;
			m_TextGlyphs.emplace_back(m_Font->GetGlyphFromChar(textChar));
		}
	}
}

void FontRenderer::Render() const
{
	constexpr int letterSpacing{ 700 };
	constexpr int wordSpacing{ 400 };

	glm::ivec2 offset{};
	size_t glyphIdx{};

	for (const char charater : m_RenderConfigs.inputText)
	{
		if (charater == ' ')
		{
			offset.x += wordSpacing;
		}
		else if (charater == '\n')
		{
			offset.x = 0;
			offset.y -= letterSpacing * 2;
		}
		else if (charater == '\t')
		{
			ENGINE_ASSERT_MSG(false, "Tab not implemented yet!");
		}
		else
		{
			DebugRenderGlyph(m_TextGlyphs[glyphIdx], offset);
			offset.x += letterSpacing;
			++glyphIdx;
		}
	}
}

void FontRenderer::DebugRenderGlyph(const GlyphData& glyph, const glm::ivec2& offset) const
{
	// Glyphbox
	if (m_RenderConfigs.drawDebugBoundBox)
	{
		const GlyphBounds glyphBounds{ GetGlyphBounds(glyph) };
		const Rect3f glyphbox{ glm::vec3{ glyphBounds.center + static_cast<glm::vec2>(offset), -2.f }, glyphBounds.size };
		Renderer2D::SetDrawColor(Color::white);
		Renderer2D::DrawRect(glyphbox);
	}

	m_RenderConfigs.useBezierCurves ? DebugRenderBezierGlyph(glyph, offset) : DebugRenderContourGlyph(glyph, offset);
}

void FontRenderer::DebugRenderContourGlyph(const GlyphData& glyph, const glm::ivec2& offset) const
{
	Renderer2D::SetLineWidth(1.5f);

	const std::vector<uint16_t>& contourEndIndices{ glyph.contourEndIndices };
	const std::vector<GlyphPoint>& points{ glyph.points };

	// Lines
	if (m_RenderConfigs.drawDebugLines)
	{
		int contourStartIdx{};
		for (size_t contourEndIdx{}; contourEndIdx < contourEndIndices.size(); ++contourEndIdx)
		{
			Renderer2D::SetDrawColor(s_ContourLineColors[contourEndIdx % s_ContourLineColors.size()]);

			const uint16_t& currentEndIdx{ contourEndIndices[contourEndIdx] };

			const size_t numPointsInContour{ static_cast<size_t>(currentEndIdx - contourStartIdx + 1) };
			const std::vector<GlyphPoint> contourPoints
			{
				points.begin() + contourStartIdx,
				points.begin() + contourStartIdx + numPointsInContour
			};

			for (size_t pointIdx{}; pointIdx < numPointsInContour; ++pointIdx)
			{
				const glm::ivec2 pointOne{ static_cast<glm::ivec2>(contourPoints[pointIdx]) + offset };
				const glm::ivec2 pointTwo{ static_cast<glm::ivec2>(contourPoints[(pointIdx + 1) % numPointsInContour]) + offset };
				Renderer2D::DrawLine(static_cast<glm::vec2>(pointOne), static_cast<glm::vec2>(pointTwo));
			}

			contourStartIdx = currentEndIdx + 1;
		}
	}

	// Points
	if (m_RenderConfigs.drawDebugPoints)
	{
		constexpr float pointSize{ 10.f };
		for (const GlyphPoint& point : points)
		{
			Renderer2D::SetDrawColor(point.onCurve ? Color::blue : Color::white);
			const glm::ivec3 drawPos{ glm::ivec3{ point.x, point.y, 1 } + glm::ivec3{ offset, 0.f } };
			Renderer2D::DrawFilledCircle(static_cast<glm::vec3>(drawPos), pointSize);
		}
	}
}

void FontRenderer::DebugRenderBezierGlyph(const GlyphData& glyph, const glm::ivec2& offset) const
{
	Renderer2D::SetLineWidth(1.5f);

	const float scale{ 1.f / m_Font->GetUnitsPerEm() };
	const std::vector<std::vector<GlyphPoint>> contours{ CreateContoursWithImpliedPoints(glyph) };

	// Lines
	if (m_RenderConfigs.drawDebugLines)
	{
		for (size_t contourIdx{}; contourIdx < contours.size(); ++contourIdx)
		{
			Renderer2D::SetDrawColor(s_ContourLineColors[contourIdx % s_ContourLineColors.size()]);

			const std::vector<GlyphPoint>& contour{ contours[contourIdx] };
			for (size_t pointIdx{}; pointIdx < contour.size(); ++pointIdx)
			{
				const GlyphPoint& pointOne{ contour[pointIdx] };
				const GlyphPoint& pointTwo{ contour[(pointIdx + 1) % contour.size()] };
				const GlyphPoint& pointThree{ contour[(pointIdx + 2) % contour.size()] };

				const glm::ivec2 p1{ glm::ivec2(pointOne) + offset };
				const glm::ivec2 p2{ glm::ivec2(pointTwo) + offset };
				const glm::ivec2 p3{ glm::ivec2(pointThree) + offset };

				if (pointOne.onCurve)
				{
					if (pointTwo.onCurve) Renderer2D::DrawLine(p1, p2);
					else
					{
						++pointIdx;
						RenderUtils2D::DrawBezier(p1, p2, p3, m_RenderConfigs.bezierCurveResolution);
					}
				}
			}
		}
	}

	// Points
	if (m_RenderConfigs.drawDebugPoints)
	{
		constexpr float pointSize{ 10.f };
		for (const std::vector<GlyphPoint>& contour : contours)
		{
			for (const GlyphPoint& point : contour)
			{
				Renderer2D::SetDrawColor(point.onCurve ? Color::blue : Color::white);
				const glm::ivec3 drawPos{ glm::ivec3{ point.x, point.y, 1 } + glm::ivec3{ offset, 0.f } };
				Renderer2D::DrawFilledCircle(static_cast<glm::vec3>(drawPos), pointSize);
			}
		}
	}
}