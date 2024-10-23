#ifndef FONTREADER_H
#define FONTREADER_H

#include <string>

#include "ByteReader.h"

class FontReader final
{
public:

	explicit FontReader(const std::string& filePath);
	~FontReader();

	FontReader(const FontReader& other) = delete;
	FontReader(FontReader&& other) noexcept = delete;
	FontReader& operator=(const FontReader& other) = delete;
	FontReader& operator=(FontReader&& other) noexcept = delete;

	std::string ReadTag();

	uint8_t ReadUint8();
	uint16_t ReadUInt16();
	uint32_t ReadUInt32();
	int8_t ReadInt8();
	int16_t ReadInt16();
	int32_t ReadInt32();
	std::vector<uint8_t> ReadBytes(size_t nrOfBytes);

	void SkipBytes(size_t nrOfBytes);
	void SetPosition(size_t position);
	size_t GetPosition();
	size_t GetFontFileSize() const;

private:

	ByteReader m_ByteReader;

};

#endif // !FONTREADER_H
