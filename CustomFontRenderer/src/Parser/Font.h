#ifndef FONT_H
#define FONT_H

#include <unordered_map>
#include <memory>
#include <string>

#include "FontReader.h"

#include "FontStructs.h"

class Font final
{
public:

	explicit Font(const std::string& filePath);
	~Font() = default;

	Font(const Font& other) = delete;
	Font(Font&& other) noexcept = delete;
	Font& operator=(const Font& other) = delete;
	Font& operator=(Font&& other) noexcept = delete;

	const GlyphData& GetGlyphFromChar(char character) const;

private:

	uint32_t GetTableOffset(const std::string& name) const;
	uint32_t GetGlyphLocFromIdx(uint32_t idx);

	void ReadFontHeader();
	void ReadTableEntries();

	void ReadHeadTable(); // Includes general information
	void ReadMaxpTable(); // Provides maximum values used in the font
	void ReadCmapTable(); // Maps Unicode values to glyph indices
	void ReadLocaTable(); // Provides offsets to glyph data in the glyf table
	void ReadGlyfTable(); // Data of the glyphs

	GlyphData ReadGlyph(uint32_t glyphIdx);
	GlyphData ReadSimpleGlyph(uint32_t glyphIdx);
	GlyphData ReadCompoundGlyph(uint32_t glyphIdx);
	std::pair<GlyphData, bool> ReadNextComponentGlyph(uint32_t glyphIdx);

	void ParseCoordinates(std::vector<int>& coordinates, const std::vector<uint8_t>& allFlags, bool readingX);

	void ReadUniCodeToIndex();

	// Printing
	void FontLog(const std::string& message);
	void PrintFontHeader();
	void PrintTableEntries();
	void PrintHeadTable();
	void PrintMaxpTable();

private:

	// File font reader
	FontReader m_FontReader;
	
	std::vector<GlyphData> m_GlyphsData;
	std::unordered_map<uint16_t, uint32_t> m_UnicodeToGlyphIdx;

	// Header
	FontHeader m_FontHeader;
	// Tables
	std::unordered_map<std::string, FontTableEntry> m_TableEntries;
	HeadTable m_FontHeadTable;
	MaxpTable m_FontMaxpTable;
	// Cmap
	LocaTable m_FontLocaTable;

	std::unordered_map<int8_t, GlyphData*> m_GlyphDataLookup;
	
};

#endif // !FONT_H