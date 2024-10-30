#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader final
{
public:

	explicit Shader(const char* vertexPath, const char* fragmentPath);
	explicit Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void Use() const;

	int GetUniformLocation(const std::string& name) const;
	void SetUniform(const std::string& name, int value) const;
	void SetUniform(const std::string& name, float value) const;
	void SetUniform(const std::string& name, const glm::vec2& vector) const;
	void SetUniform(const std::string& name, const glm::vec3& vector) const;
	void SetUniform(const std::string& name, const glm::vec4& vector) const;
	void SetUniform(const std::string& name, const glm::mat4& matrix) const;

private:

	void CompileShader(GLuint shaderID, const std::string& source);

private:

	GLuint programID;

};

#endif // !SHADER_H