#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include <vector>
#include <string>

#include "Structs.h"

class FontParser;

struct GlyphData;

class FontRenderer final
{
public:

	explicit FontRenderer(FontParser* parser, const std::string& text = "default");
	~FontRenderer() = default;

	FontRenderer(const FontRenderer& other) = delete;
	FontRenderer(FontRenderer&& other) noexcept = delete;
	FontRenderer& operator=(const FontRenderer& other) = delete;
	FontRenderer& operator=(FontRenderer&& other) noexcept = delete;

	void Render() const;


private:

	void LoadTextGlyphs();

	void DrawGlyph(const GlyphData& glyphData, const Point2i& offset) const;

private:

	static std::vector<Color4f> s_ContourLineColors;

	FontParser* m_pFontParser;
	std::vector<GlyphData> m_TextGlyphs;
	std::string m_Text;

};

#endif // !FONTRENDERER_H