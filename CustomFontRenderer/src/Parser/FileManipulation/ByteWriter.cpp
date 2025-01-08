#include "ByteWriter.h"

ByteWriter::ByteWriter(const std::string& filePath)
    : m_File{ filePath, std::ios::out | std::ios::binary }
    , m_FilePath{ filePath }
{
    if (!m_File)
    {
        throw std::runtime_error{ "Unable to open file!" };
    }
}

ByteWriter::~ByteWriter()
{
    if (m_File.is_open())
    {
        m_File.close();
    }
}

void ByteWriter::Open()
{
    if (!m_File.is_open())
    {
        m_File.open(m_FilePath, std::ios::binary);
    }
}

void ByteWriter::Close()
{
    if (m_File.is_open())
    {
        m_File.close();
    }
}

void ByteWriter::WriteByte(uint8_t value)
{
    m_File.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void ByteWriter::WriteBytes(const std::vector<uint8_t>& values)
{
    for (uint8_t value : values)
    {
        WriteByte(value);
    }
}