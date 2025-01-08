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

	const std::string& GetFilePath() const;

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

	void SkipBytes(const std::streampos& nrOfBytes);
	void SetPosition(const std::streampos& position);
	std::streampos GetPosition();
	const std::streampos& GetFileSize() const;

	static int16_t SwapEndianness(int16_t value);
    static uint16_t SwapEndianness(uint16_t value);
	static int32_t SwapEndianness(int32_t value);
    static uint32_t SwapEndianness(uint32_t value);
	static int64_t SwapEndianness(int64_t value);
	static uint64_t SwapEndianness(uint64_t value);

	static void SwapEndianness(int16_t* value);
	static void SwapEndianness(uint16_t* value);
	static void SwapEndianness(int32_t* value);
	static void SwapEndianness(uint32_t* value);
	static void SwapEndianness(int64_t* value);
	static void SwapEndianness(uint64_t* value);

	static bool IsBitSet(uint8_t byte, size_t bitIdx);
	static bool IsBitSet(uint16_t twobyte, size_t bitIdx);

private:

	std::ifstream m_File;
	std::string m_FilePath;
	std::streampos m_FileSize;

};

#endif // !BYTEREADER_H