#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include <vector>
#include <string>
#include <memory>

#include <Engine.h>

class Font;
struct GlyphData;

struct FontRenderConfigs
{
	std::string fontPath{};
	std::string inputText{ "Default Text" };
	bool drawDebugLines{ true };
	bool drawDebugPoints{ false };
	bool drawDebugBoundBox{ false };

	bool useBezierCurves{ false };
	uint32_t bezierCurveResolution{ 5 };

	bool operator==(const FontRenderConfigs& other) const
	{
		return inputText == other.inputText &&
			drawDebugLines == other.drawDebugLines &&
			drawDebugPoints == other.drawDebugPoints &&
			drawDebugBoundBox == other.drawDebugBoundBox &&
			useBezierCurves == other.useBezierCurves &&
			bezierCurveResolution == other.bezierCurveResolution;
	}

	bool operator!=(const FontRenderConfigs& other) const
	{
		return !(*this == other);
	}
};

class FontRenderer final
{
public:

	FontRenderer();
	FontRenderer(const FontRenderConfigs& configs);
	explicit FontRenderer(std::shared_ptr<Font> font);
	~FontRenderer();

	void SetRenderConfigs(const FontRenderConfigs& configs);
	void UpdateRenderData(const FontRenderConfigs& newConfigs, bool forceTextReload = false, bool forceFontReload = false);

	void DebugRenderGlyph(const GlyphData& glyph, const glm::ivec2& offset) const;
	void DebugRenderContourGlyph(const GlyphData& glyph, const glm::ivec2& offset) const;
	void DebugRenderBezierGlyph(const GlyphData& glyph, const glm::ivec2& offset) const;
	void Render() const;

private:

	static const std::vector<glm::vec4> s_ContourLineColors;

	std::shared_ptr<Font> m_Font;

	FontRenderConfigs m_RenderConfigs;
	std::vector<GlyphData> m_TextGlyphs;

};

#endif // !FONTRENDERER_H