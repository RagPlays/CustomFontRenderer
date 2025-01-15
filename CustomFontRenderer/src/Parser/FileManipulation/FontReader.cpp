#include "FontReader.h"

FontReader::FontReader(const std::string& filePath)
	: m_ByteReader{ filePath }
{
}

FontReader::~FontReader()
{
}

void FontReader::Open()
{
	m_ByteReader.Open();
}

void FontReader::Close()
{
	m_ByteReader.Close();
}

const std::string& FontReader::GetFontPath() const
{
	return m_ByteReader.GetFilePath();
}

std::string FontReader::ReadTag()
{
	const std::vector<uint8_t> bytes{ ReadBytes(4) };
	return std::string(bytes.begin(), bytes.end());
}

void FontReader::ReadTag(std::string& tag)
{
	tag.resize(4);
	const std::vector<uint8_t> bytes{ ReadBytes(4) };
	std::memcpy(&tag[0], bytes.data(), 4);
}

void FontReader::ReadTag(char* data)
{
	const std::vector<uint8_t> bytes{ ReadBytes(4) };
	std::memcpy(data, bytes.data(), 4);
}

double FontReader::ReadFixedPoint2Dot14()
{
	return UInt16ToFixedPoint2Dot14(ReadUInt16());
}

double FontReader::UInt16ToFixedPoint2Dot14(uint16_t raw)
{
	return static_cast<uint16_t>(raw) / static_cast<double>(1 << 14);
}

void FontReader::ReadUInt8(uint8_t* data)
{
	m_ByteReader.ReadUInt8(data);
}

uint8_t FontReader::ReadUInt8()
{
	return m_ByteReader.ReadUInt8();
}

uint16_t FontReader::ReadUInt16()
{
	return ByteReader::SwapEndianness(m_ByteReader.ReadUInt16());
}

void FontReader::ReadUInt16(uint16_t* data)
{
	m_ByteReader.ReadUInt16(data);
	ByteReader::SwapEndianness(data);
}

uint32_t FontReader::ReadUInt32()
{
	return ByteReader::SwapEndianness(m_ByteReader.ReadUInt32());
}

void FontReader::ReadUInt32(uint32_t* data)
{
	m_ByteReader.ReadUInt32(data);
	ByteReader::SwapEndianness(data);
}

uint64_t FontReader::ReadUInt64()
{
	return ByteReader::SwapEndianness(m_ByteReader.ReadUInt64());
}

void FontReader::ReadUInt64(uint64_t* data)
{
	m_ByteReader.ReadUInt64(data);
	ByteReader::SwapEndianness(data);
}

int8_t FontReader::ReadInt8()
{
	return m_ByteReader.ReadInt8();
}

void FontReader::ReadInt8(int8_t* data)
{
	m_ByteReader.ReadInt8(data);
}

int16_t FontReader::ReadInt16()
{
	return ByteReader::SwapEndianness(m_ByteReader.ReadInt16());
}

void FontReader::ReadInt16(int16_t* data)
{
	m_ByteReader.ReadInt16(data);
	ByteReader::SwapEndianness(data);
}

int32_t FontReader::ReadInt32()
{
	return ByteReader::SwapEndianness(m_ByteReader.ReadInt32());
}

void FontReader::ReadInt32(int32_t* data)
{
	m_ByteReader.ReadInt32(data);
	ByteReader::SwapEndianness(data);
}

int64_t FontReader::ReadInt64()
{
	return ByteReader::SwapEndianness(m_ByteReader.ReadInt64());
}

void FontReader::ReadInt64(int64_t* data)
{
	m_ByteReader.ReadInt64(data);
	ByteReader::SwapEndianness(data);
}

std::vector<uint8_t> FontReader::ReadBytes(size_t nrOfBytes)
{
	return m_ByteReader.ReadBytes(nrOfBytes);
}

void FontReader::ReadBytes(uint8_t* data, size_t size)
{
	m_ByteReader.ReadBuffer(data, size);
}

void FontReader::ReadBuffer(void* data, size_t size)
{
	m_ByteReader.ReadBuffer(data, size);
}

void FontReader::SkipBytes(size_t nrOfBytes)
{
	m_ByteReader.SkipBytes(static_cast<std::streampos>(nrOfBytes));
}

void FontReader::SetPosition(size_t position)
{
	m_ByteReader.SetPosition(static_cast<std::streampos>(position));
}

size_t FontReader::GetPosition()
{
	return static_cast<size_t>(m_ByteReader.GetPosition());
}

size_t FontReader::GetFontFileSize() const
{
	return static_cast<size_t>(m_ByteReader.GetFileSize());
}