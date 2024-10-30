#ifndef BYTEREADER_H
#define BYTEREADER_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <iosfwd>

class ByteReader final
{
public:

	explicit ByteReader(const std::string& filePath);
	~ByteReader();

	ByteReader(const ByteReader& other) = delete;
	ByteReader(ByteReader&& other) noexcept = delete;
	ByteReader& operator=(const ByteReader& other) = delete;
	ByteReader& operator=(ByteReader&& other) noexcept = delete;

	void Open();
	void Close();

	uint8_t ReadUInt8();
	uint16_t ReadUInt16();
	uint32_t ReadUInt32();

	int8_t ReadInt8();
	int16_t ReadInt16();
	int32_t ReadInt32();

	std::vector<uint8_t> ReadBytes(size_t nrOfBytes);

	void SkipBytes(std::streampos nrOfBytes);
	void SetPosition(std::streampos position);
	std::streampos GetPosition();
	const std::streampos& GetFileSize() const;

    static uint16_t SwapEndianness(uint16_t value);
    static uint32_t SwapEndianness(uint32_t value);

	static int16_t SwapEndianness(int16_t value);
	static int32_t SwapEndianness(int32_t value);

	static bool IsBitSet(uint8_t byte, size_t bitIdx);
	static bool IsBitSet(uint16_t twobyte, size_t bitIdx);

private:

	std::ifstream m_File;
	std::string m_FilePath;
	std::streampos m_FileSize;

};

#endif // !BYTEREADER_H