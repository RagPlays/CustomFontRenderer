#include "Shader.h"

#include <gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode{};
    std::string fragmentCode{};

    std::ifstream vShaderFile{ vertexPath };
    std::ifstream fShaderFile{ fragmentPath };

    // Ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        std::stringstream vShaderStream{};
        std::stringstream fShaderStream{};
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << "\n";
        std::cerr << "Exception: " << e.what() << "\n";
    }

    // 2. Compile shaders
    const uint32_t vertexShader{ glCreateShader(GL_VERTEX_SHADER) };
    const uint32_t fragmentShader{ glCreateShader(GL_FRAGMENT_SHADER) };

    CompileShader(vertexShader, vertexCode);
    CompileShader(fragmentShader, fragmentCode);

    // 3. Create shader program
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    // 4. Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : Shader{ vertexPath.c_str(), fragmentPath.c_str() }
{
}

Shader::~Shader()
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}

void Shader::Use() const
{
    glUseProgram(programID);
}

int Shader::GetUniformLocation(const std::string& name) const
{
    int location{ glGetUniformLocation(programID, name.c_str()) };
    if (location == -1)
    {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist or wasn't used in the shader!" << "\n";
    }
    return location;
}

void Shader::SetUniform(const std::string& name, int value) const
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform(const std::string& name, float value) const
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& vector) const
{
    glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(vector));
}

void Shader::SetUniform(const std::string& name, const glm::vec3& vector) const
{
    glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vector));
}

void Shader::SetUniform(const std::string& name, const glm::vec4& vector) const
{
    glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(vector));
}

void Shader::SetUniform(const std::string& name, const glm::mat4& matrix) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::CompileShader(GLuint shaderID, const std::string& source)
{
    const char* shaderCode = source.c_str();
    glShaderSource(shaderID, 1, &shaderCode, nullptr);
    glCompileShader(shaderID);

    // Check for compile time errors
    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}