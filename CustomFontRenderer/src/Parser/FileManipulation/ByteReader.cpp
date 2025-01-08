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
    Close();
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

const std::string& ByteReader::GetFilePath() const
{
    return m_FilePath;
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

void ByteReader::ReadUInt8(uint8_t* data)
{
    assert(data);
    if (!m_File.read(reinterpret_cast<char*>(data), sizeof(uint8_t)))
    {
        throw std::runtime_error{ "Failed to read uint8_t into provided buffer." };
    }
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

void ByteReader::ReadUInt16(uint16_t* data)
{
    assert(data);
    if (!m_File.read(reinterpret_cast<char*>(data), sizeof(uint16_t)))
    {
        throw std::runtime_error{ "Failed to read uint16 into provided buffer." };
    }
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

void ByteReader::ReadUInt32(uint32_t* data)
{
    assert(data);
    if (!m_File.read(reinterpret_cast<char*>(data), sizeof(uint32_t)))
    {
        throw std::runtime_error{ "Failed to read uint32 into provided buffer." };
    }
}

uint64_t ByteReader::ReadUInt64()
{
    uint64_t eightBytes{};
    if (!m_File.read(reinterpret_cast<char*>(&eightBytes), sizeof(eightBytes)))
    {
        throw std::runtime_error{ "Failed to read uint64 from file." };
    }
    return eightBytes;
}

void ByteReader::ReadUInt64(uint64_t* data)
{
    assert(data);
    if (!m_File.read(reinterpret_cast<char*>(data), sizeof(uint64_t)))
    {
        throw std::runtime_error{ "Failed to read uint32 into provided buffer." };
    }
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

void ByteReader::ReadInt8(int8_t* data)
{
    assert(data);
    if (!m_File.read(reinterpret_cast<char*>(data), sizeof(int8_t)))
    {
        throw std::runtime_error{ "Failed to read int8_t into provided buffer." };
    }
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

void ByteReader::ReadInt16(int16_t* data)
{
    assert(data);
    if (!m_File.read(reinterpret_cast<char*>(data), sizeof(int16_t)))
    {
        throw std::runtime_error{ "Failed to read int16_t into provided buffer." };
    }
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

void ByteReader::ReadInt32(int32_t* data)
{
    assert(data);
    if (!m_File.read(reinterpret_cast<char*>(data), sizeof(int32_t)))
    {
        throw std::runtime_error{ "Failed to read int32_t into provided buffer." };
    }
}

int64_t ByteReader::ReadInt64()
{
    int32_t eightBytes{};
    if (!m_File.read(reinterpret_cast<char*>(&eightBytes), sizeof(eightBytes)))
    {
        throw std::runtime_error{ "Failed to read int32_t from file." };
    }
    return eightBytes;
}

void ByteReader::ReadInt64(int64_t* data)
{
    assert(data);
    if (!m_File.read(reinterpret_cast<char*>(data), sizeof(int64_t)))
    {
        throw std::runtime_error{ "Failed to read int32_t into provided buffer." };
    }
}

std::vector<uint8_t> ByteReader::ReadBytes(size_t size)
{
    std::vector<uint8_t> buffer(size);
    if (!m_File.read(reinterpret_cast<char*>(buffer.data()), size))
    {
        throw std::runtime_error{ "Failed to read bytes!" };
    }
    return buffer;
}

void ByteReader::ReadBytes(uint8_t* data, size_t size)
{
    assert(data);
    if (!m_File.read(reinterpret_cast<char*>(data), size))
    {
        throw std::runtime_error{ "Failed to read bytes into provided buffer!" };
    }
}

void ByteReader::ReadBuffer(void* data, size_t size)
{
    assert(data);
    if (!m_File.read(reinterpret_cast<char*>(data), size))
    {
        throw std::runtime_error{ "Failed to read buffer from file." };
    }
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

int64_t ByteReader::SwapEndianness(int64_t value)
{
    return static_cast<int64_t>(SwapEndianness(static_cast<uint64_t>(value)));
}

uint64_t ByteReader::SwapEndianness(uint64_t value)
{
    return
        (value >> 56) |
        ((value >> 40) & 0x000000000000FF00ULL) |
        ((value >> 24) & 0x0000000000FF0000ULL) |
        ((value >> 8) & 0x00000000FF000000ULL) |
        ((value << 8) & 0x000000FF00000000ULL) |
        ((value << 24) & 0x0000FF0000000000ULL) |
        ((value << 40) & 0x00FF000000000000ULL) |
        (value << 56);
}

void ByteReader::SwapEndianness(int16_t* value)
{
    assert(value);
    *value = static_cast<int16_t>(SwapEndianness(static_cast<uint16_t>(*value)));
}

void ByteReader::SwapEndianness(uint16_t* value)
{
    assert(value);
    *value = (*value >> 8) | (*value << 8);
}

void ByteReader::SwapEndianness(int32_t* value)
{
    assert(value);
    *value = static_cast<int32_t>(SwapEndianness(static_cast<uint32_t>(*value)));
}

void ByteReader::SwapEndianness(uint32_t* value)
{
    assert(value);
    *value =
        (*value >> 24) |
        ((*value >> 8) & 0x0000FF00) |
        ((*value << 8) & 0x00FF0000) |
        (*value << 24);
}

void ByteReader::SwapEndianness(int64_t* value)
{
    assert(value);
    *value = static_cast<int64_t>(SwapEndianness(static_cast<uint64_t>(*value)));
}

void ByteReader::SwapEndianness(uint64_t* value)
{
    assert(value);
    *value =
        (*value >> 56) |
        ((*value >> 40) & 0x000000000000FF00ULL) |
        ((*value >> 24) & 0x0000000000FF0000ULL) |
        ((*value >> 8) & 0x00000000FF000000ULL) |
        ((*value << 8) & 0x000000FF00000000ULL) |
        ((*value << 24) & 0x0000FF0000000000ULL) |
        ((*value << 40) & 0x00FF000000000000ULL) |
        (*value << 56);
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