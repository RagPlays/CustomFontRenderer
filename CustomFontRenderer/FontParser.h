#ifndef CUSTOMTTFPARSER_H
#define CUSTOMTTFPARSER_H

#include <unordered_map>
#include <memory>
#include <string>

#include "FontReader.h"

#include "FontStructs.h"

class FontParser final
{
public:

	explicit FontParser(const std::string& filePath);
	~FontParser() = default;

	FontParser(const FontParser& other) = delete;
	FontParser(FontParser&& other) noexcept = delete;
	FontParser& operator=(const FontParser& other) = delete;
	FontParser& operator=(FontParser&& other) noexcept = delete;

	const GlyphData& GetGlyphFromChar(char character) const;
	const std::vector<GlyphData>& GetGlyphsData() const;

private:

	void ReadTableLocations();
	void ReadGlyphLocations();
	void ReadGlyphs();
	void ReadUniCodeToIndex();

	GlyphData ReadGlyph(uint32_t glyphPos);
	GlyphData ReadSimpleGlyph(uint32_t glyphPos);
	GlyphData ReadCompoundGlyph(uint32_t glyphPos);
	std::pair<GlyphData, bool> ReadNextComponentGlyph(uint32_t glyphPos);
	std::vector<int> ParseCoordinates(const std::vector<uint8_t>& allFlags, bool readingX);
	void ParseLog(const std::string& message);
	void ParseLogFlag(size_t flagIdx, uint8_t flag);

private:

	std::vector<GlyphData> m_GlyphsData;
	std::unordered_map<std::string, TableData> m_TableLocationsLookup;
	std::unordered_map<uint32_t, uint32_t> m_GlyphUnicodeToIndexLookup;
	std::vector<uint32_t> m_GlyphLocations;

	FontReader m_FontReader;
};

#endif // !CUSTOMTTFPARSER_H