#ifndef FONTREADER_H
#define FONTREADER_H

#include <vector>
#include <string>
#include <cstdint>

#include "Parser/FileManipulation/ByteReader.h"

class FontReader final
{
public:

	explicit FontReader(const std::string& filePath);
	~FontReader();

	FontReader(const FontReader& other) = delete;
	FontReader(FontReader&& other) noexcept = delete;
	FontReader& operator=(const FontReader& other) = delete;
	FontReader& operator=(FontReader&& other) noexcept = delete;

	void Open();
	void Close();

	const std::string& GetFontPath() const;

	std::string ReadTag();
	void ReadTag(std::string& tag);
	void ReadTag(char* data);
	
	double ReadFixedPoint2Dot14();
	static double UInt16ToFixedPoint2Dot14(uint16_t raw);

	uint8_t ReadUInt8();
	void ReadUInt8(uint8_t* data);
	uint16_t ReadUInt16();
	void ReadUInt16(uint16_t* data);
	uint32_t ReadUInt32();
	void ReadUInt32(uint32_t* data);
	uint64_t ReadUInt64();
	void ReadUInt64(uint64_t* data);

	int8_t ReadInt8();
	void ReadInt8(int8_t* data);
	int16_t ReadInt16();
	void ReadInt16(int16_t* data);
	int32_t ReadInt32();
	void ReadInt32(int32_t* data);
	int64_t ReadInt64();
	void ReadInt64(int64_t* data);

	std::vector<uint8_t> ReadBytes(size_t size);
	void ReadBytes(uint8_t* data, size_t size);
	void ReadBuffer(void* data, size_t size);

	void SkipBytes(size_t nrOfBytes);
	void SetPosition(size_t position);
	size_t GetPosition();
	size_t GetFontFileSize() const;

private:

	ByteReader m_ByteReader;

};

#endif // !FONTREADER_H
