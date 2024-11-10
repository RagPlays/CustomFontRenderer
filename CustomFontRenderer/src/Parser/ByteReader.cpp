#include "ByteReader.h"

#include <cassert>
#include <cstdint>
#include <iosfwd>
#include <stdexcept>
#include <string>
#include <vector>

ByteReader::ByteReader(const std::string& filePath)
    : m_File{ filePath, std::ios::in | std::ios::binary }
    , m_FilePath{ filePath }
    , m_FileSize{}
{
    if (!m_File)
    {
        throw std::runtime_error{ "Unable to open file!" };
    }

    m_File.seekg(0, std::ios::end);
    m_FileSize = GetPosition();
    m_File.seekg(0, std::ios::beg);
}

ByteReader::~ByteReader()
{
    if (m_File.is_open())
    {
        m_File.close();
    }
}

void ByteReader::Open()
{
    if (!m_File.is_open())
    {
        m_File.open(m_FilePath, std::ios::binary);
    }
}

void ByteReader::Close()
{
    if (m_File.is_open())
    {
        m_File.close();
    }
}

uint8_t ByteReader::ReadUInt8()
{
    uint8_t byte{};
    if (!m_File.read(reinterpret_cast<char*>(&byte), sizeof(byte)))
    {
        throw std::runtime_error{ "Failed to read uint8_t from file." };
    }
    return byte;
}

uint16_t ByteReader::ReadUInt16()
{
    uint16_t twoBytes{};
    if (!m_File.read(reinterpret_cast<char*>(&twoBytes), sizeof(twoBytes)))
    {
        throw std::runtime_error{ "Failed to read uint16 from file." };
    }
    return twoBytes;
}

uint32_t ByteReader::ReadUInt32()
{
    uint32_t fourBytes{};
    if (!m_File.read(reinterpret_cast<char*>(&fourBytes), sizeof(fourBytes)))
    {
        throw std::runtime_error{ "Failed to read uint32 from file." };
    }
    return fourBytes;
}

int8_t ByteReader::ReadInt8()
{
    int8_t byte{};
    if (!m_File.read(reinterpret_cast<char*>(&byte), sizeof(byte)))
    {
        throw std::runtime_error{ "Failed to read int8_t from file." };
    }
    return byte;
}

int16_t ByteReader::ReadInt16()
{
    int16_t twoBytes{};
    if (!m_File.read(reinterpret_cast<char*>(&twoBytes), sizeof(twoBytes)))
    {
        throw std::runtime_error{ "Failed to read int16_t from file." };
    }
    return twoBytes;
}

int32_t ByteReader::ReadInt32()
{
    int32_t fourBytes{};
    if (!m_File.read(reinterpret_cast<char*>(&fourBytes), sizeof(fourBytes)))
    {
        throw std::runtime_error{ "Failed to read int32_t from file." };
    }
    return fourBytes;
}

std::vector<uint8_t> ByteReader::ReadBytes(size_t nrOfBytes)
{
    std::vector<uint8_t> buffer(nrOfBytes);
    if (!m_File.read(reinterpret_cast<char*>(buffer.data()), nrOfBytes))
    {
        throw std::runtime_error{ "Failed to read bytes!" };
    }
    return buffer;
}

void ByteReader::SkipBytes(const std::streampos& nrOfBytes)
{
    m_File.seekg(nrOfBytes, std::ios::cur);
    if (!m_File)
    {
        throw std::runtime_error{ "Failed to skip bytes!" };
    }
}

void ByteReader::SetPosition(const std::streampos& position)
{
    m_File.seekg(position, std::ios::beg);
    if (!m_File)
    {
        throw std::runtime_error{ "Failed to set position!" };
    }
}

std::streampos ByteReader::GetPosition()
{
    const std::streampos position{ m_File.tellg() };
    if (position == -1)
    {
        throw std::runtime_error{ "Failed to get position!" };
    }
    return position;
}

const std::streampos& ByteReader::GetFileSize() const
{
    return m_FileSize;
}

int16_t ByteReader::SwapEndianness(int16_t value)
{
    return static_cast<int16_t>(SwapEndianness(static_cast<uint16_t>(value)));
}

uint16_t ByteReader::SwapEndianness(uint16_t value)
{
    return (value >> 8) | (value << 8);
}

int32_t ByteReader::SwapEndianness(int32_t value)
{
    return static_cast<int32_t>(SwapEndianness(static_cast<uint32_t>(value)));
}

uint32_t ByteReader::SwapEndianness(uint32_t value)
{
    return
        (value >> 24) |
        ((value >> 8) & 0x0000FF00) |
        ((value << 8) & 0x00FF0000) |
        (value << 24);
}

bool ByteReader::IsBitSet(uint8_t byte, size_t bitIdx)
{
    assert(bitIdx < sizeof(byte) * 8);
    return ((byte >> bitIdx) & 1) == 1;
}

bool ByteReader::IsBitSet(uint16_t twobyte, size_t bitIdx)
{
    assert(bitIdx < sizeof(twobyte) * 8);
    return ((twobyte >> bitIdx) & 1) == 1;
}