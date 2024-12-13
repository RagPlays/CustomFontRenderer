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
	std::string inputText{ "Default Text" };
	bool drawDebugLines{ true };
	bool drawDebugPoints{ false };
	bool drawDebugBoundBox{ false };
};

class FontRenderer final
{
public:

	explicit FontRenderer(std::shared_ptr<Font> font);
	~FontRenderer();

	void SetRenderConfigs(const FontRenderConfigs& configs);

	void RenderGlyph(const GlyphData& glyph, const glm::ivec2& offset) const;
	void Render() const;

private:

	static std::vector<glm::vec4> s_ContourLineColors;

	std::shared_ptr<Font> m_Font;
	FontRenderConfigs m_RenderConfigs;

};

#endif // !FONTRENDERER_H