#include "FontReader.h"

FontReader::FontReader(const std::string& filePath)
	: m_ByteReader{ filePath }
{
}

FontReader::~FontReader()
{
}

std::string FontReader::ReadTag()
{
	const std::vector<uint8_t> dataVec{ m_ByteReader.ReadBytes(4) };
	return std::string(dataVec.begin(), dataVec.end());
}

uint8_t FontReader::ReadUint8()
{
	return m_ByteReader.ReadUInt8();
}

uint16_t FontReader::ReadUInt16()
{
	return m_ByteReader.SwapEndianness(m_ByteReader.ReadUInt16());
}

uint32_t FontReader::ReadUInt32()
{
	return m_ByteReader.SwapEndianness(m_ByteReader.ReadUInt32());
}

int8_t FontReader::ReadInt8()
{
	return m_ByteReader.ReadInt8();
}

int16_t FontReader::ReadInt16()
{
	return m_ByteReader.SwapEndianness(m_ByteReader.ReadInt16());
}

int32_t FontReader::ReadInt32()
{
	return m_ByteReader.SwapEndianness(m_ByteReader.ReadInt32());
}

std::vector<uint8_t> FontReader::ReadBytes(size_t nrOfBytes)
{
	return m_ByteReader.ReadBytes(nrOfBytes);
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