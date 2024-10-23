#ifndef BYTEWRITER_H
#define BYTEWRITER_H

#include <string>
#include <fstream>
#include <vector>

class ByteWriter final
{
public:

    explicit ByteWriter(const std::string& filePath);
    ~ByteWriter();

    ByteWriter(const ByteWriter& other) = delete;
    ByteWriter(ByteWriter&& other) noexcept = delete;
    ByteWriter& operator=(const ByteWriter& other) = delete;
    ByteWriter& operator=(ByteWriter&& other) noexcept = delete;

    void Open();
    void Close();

    void WriteByte(uint8_t value);
    void WriteBytes(const std::vector<uint8_t>& values);

private:

    std::ofstream m_File;
    std::string m_FilePath;

};

#endif // !BYTEWRITER_H