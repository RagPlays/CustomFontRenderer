//#include "FontData.h"
//
//FontData::FontData(const std::vector<GlyphData>* const glyphs, const std::unordered_map<uint32_t, uint32_t>* const glyphIdxToUnicode, int unitsPerEm)
//	: missingGlyph{ nullptr }
//	, unitsPerEm{ unitsPerEm }
//	, glyphLookup{}
//{
//	if (!glyphs) throw std::runtime_error{ "No valid glyphData vector provided!" };
//
//	missingGlyph = &((*glyphs).at(0));
//
//	for (const auto& [glyphIdx, uniCode] : *glyphIdxToUnicode)
//	{
//	}
//
//	for (const auto& glyph : *glyphs)
//	{
//		glyphLookup[glyph.unicodeValue] =  &glyph;
//	}
//
//	if (missingGlyph == nullptr) throw std::runtime_error{ "No missing character glyph provided!" };
//}
//
//const GlyphData* FontData::TryGetGlyphData(uint16_t unicode) const
//{
//	const auto it{ glyphLookup.find(unicode) };
//	if (it != glyphLookup.end()) return it->second;
//	return missingGlyph;
//}