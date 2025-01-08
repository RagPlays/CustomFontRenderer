//#ifndef FONTDATA_H
//#define FONTDATA_H
//
//#include "FontStructs.h"
//
//class FontData final
//{
//public:
//
//	explicit FontData(const std::vector<GlyphData>* const glyphs, const std::unordered_map<uint32_t, uint32_t>* const glyphIdxToUnicode, int unitsPerEm);
//	~FontData() = default;
//
//	const GlyphData* TryGetGlyphData(uint16_t unicode) const;
//
//private:
//
//	const GlyphData* missingGlyph;
//	int unitsPerEm;
//	std::unordered_map<uint16_t, const GlyphData*> glyphLookup;
//
//};
//
//#endif //!FONTDATA_H