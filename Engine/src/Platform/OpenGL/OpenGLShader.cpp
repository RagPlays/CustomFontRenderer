#include "enginepch.h"
#include "OpenGLShader.h"

#include <array>
#include <fstream>
#include <filesystem>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
	static unsigned int ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		// pixel shader and fragment shader are the same
		else if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		ENGINE_CORE_ASSERT_MSG(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
		: m_RendererID{}
		, m_FilePath{ filepath }
		, m_Name{}
	{
		ENGINE_PROFILE_FUNCTION();

		// Read, preprocess, compile
		const std::string fileSource{ ReadFile(filepath) };
		const auto shaderSources{ PreProcess(fileSource) };
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash{ filepath.find_last_of("/\\") };
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		const auto lastDot{ filepath.rfind('.') };
		m_Name = filepath.substr(lastSlash, (lastDot == std::string::npos) ? filepath.size() - lastSlash : lastDot - lastSlash);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_RendererID{}
		, m_FilePath{}
		, m_Name{ name }
	{
		ENGINE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources{};
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;

		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		ENGINE_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		ENGINE_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		ENGINE_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value) const
	{
		ENGINE_PROFILE_FUNCTION();

		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count) const
	{
		ENGINE_PROFILE_FUNCTION();

		glUniform1iv(GetUniformLocation(name), count, values);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value) const
	{
		ENGINE_PROFILE_FUNCTION();

		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) const
	{
		ENGINE_PROFILE_FUNCTION();

		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) const
	{
		ENGINE_PROFILE_FUNCTION();

		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) const
	{
		ENGINE_PROFILE_FUNCTION();

		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value) const
	{
		ENGINE_PROFILE_FUNCTION();

		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) const
	{
		ENGINE_PROFILE_FUNCTION();

		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	bool OpenGLShader::UniformExists(const std::string& name) const
	{
		const auto it{ m_UniformLocationCache.find(name) };
		if (it != m_UniformLocationCache.end()) return true;

		const int location{ glGetUniformLocation(m_RendererID, name.c_str()) };
		if (location == -1) return false;

		m_UniformLocationCache.insert({ name, location });
		return true;
	}

	const std::string& OpenGLShader::GetName() const
	{
		return m_Name;
	}

	// PRIVATE //
	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		ENGINE_PROFILE_FUNCTION();

		std::string result{};
		std::ifstream input{ filepath, std::ios::in | std::ios::binary }; // ifstream closes itself due to RAII
		if (!input.good())
		{
			ENGINE_CORE_ERROR("Could not open file '{0}'", filepath);
			ENGINE_CORE_ASSERT(false);
			return result;
		}

		input.seekg(0, std::ios::end);
		const size_t size{ static_cast<size_t>(input.tellg()) };
		if (size == -1)
		{
			ENGINE_CORE_ERROR("Could not read from file '{0}'", filepath);
			ENGINE_CORE_ASSERT(false);
			return result;
		}

		result.resize(size);
		input.seekg(0, std::ios::beg);
		input.read(&result[0], size);

		return result;
	}

	std::unordered_map<unsigned int, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		ENGINE_PROFILE_FUNCTION();

		std::unordered_map<unsigned int, std::string> shaderSources{};

		const char* typeToken{ "#type" };
		const size_t typeTokenLength{ std::strlen(typeToken) };

		//Start of shader type declaration line
		size_t currentPos{ source.find(typeToken, 0) };
		while (currentPos != std::string::npos)
		{
			//End of shader type declaration line
			const size_t eol{ source.find_first_of("\r\n", currentPos) };
			ENGINE_CORE_ASSERT_MSG(eol != std::string::npos, "Syntax error");

			//Start of shader type name (after "#type " keyword)
			const size_t begin{ currentPos + typeTokenLength + 1 };
			const std::string type{ source.substr(begin, eol - begin) };
			const unsigned int typeId{ ShaderTypeFromString(type) };
			ENGINE_CORE_ASSERT_MSG(typeId, "Invalid shader type specified");

			//Start of shader code after shader type declaration line
			const size_t nextLinePos{ source.find_first_not_of("\r\n", eol) };
			ENGINE_CORE_ASSERT_MSG(nextLinePos != std::string::npos, "Syntax error");

			//Start of next shader type declaration line
			currentPos = source.find(typeToken, nextLinePos);

			if (currentPos == std::string::npos) shaderSources[typeId] = source.substr(nextLinePos);
			else shaderSources[typeId] = source.substr(nextLinePos, currentPos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<unsigned int, std::string>& shaderSources)
	{
		ENGINE_PROFILE_FUNCTION();

		// Create programm
		unsigned int programId{ glCreateProgram() };

		ENGINE_CORE_ASSERT_MSG(shaderSources.size() <= 2, "Engine only supports 2 shaders for now");

		std::array<unsigned int, 2> glShaderIDs{};
		int glShaderIDIndex{};

		for (const auto& [type, source] : shaderSources)
		{
			const unsigned int shaderId{ glCreateShader(type) };

			const char* sourceCStr{ source.c_str() };
			glShaderSource(shaderId, 1, &sourceCStr, nullptr);
			glCompileShader(shaderId);

			// check compile status
			int isCompiled{};
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				int maxLength{};
				glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength including the NULL character
				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shaderId, maxLength, &maxLength, infoLog.data());

				// Cleanup and error report
				glDeleteShader(shaderId);
				ENGINE_CORE_ERROR("{0}", infoLog.data());
				ENGINE_CORE_ASSERT_MSG(false, "Shader compilation failure!");
				m_RendererID = 0;
				return;
			}

			// Attach the shader when compiling succesfull
			glAttachShader(programId, shaderId);
			glShaderIDs[glShaderIDIndex++] = shaderId;
		}

		// Link the programm
		glLinkProgram(programId);

		// Check link status
		int isLinked{};
		glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			int maxLength{};
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength including the NULL character
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(programId, maxLength, &maxLength, infoLog.data());

			// We don't need to programm and shaders anymore
			glDeleteProgram(programId);
			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			ENGINE_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());
			ENGINE_CORE_ASSERT(false);
			m_RendererID = 0;
			return;
		}

		// Detach shaders again after successful link because there in the programm
		for (auto id : glShaderIDs)
		{
			glDetachShader(programId, id);
			glDeleteShader(id);
		}

		// Setting ID when everything went well
		m_RendererID = programId;
	}

	int OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		const auto it{ m_UniformLocationCache.find(name) };
		if (it != m_UniformLocationCache.end()) return it->second;

		const int location{ glGetUniformLocation(m_RendererID, name.c_str()) };
#if defined ENGINE_DEBUG
		if (location == -1)
		{
			ENGINE_CORE_ERROR("Could not find \"{0}\", in shader \"{1}\" with id {2}", name, m_Name, m_RendererID);
		}
#endif
		ENGINE_CORE_ASSERT_MSG(location != -1, "Uniform not found!");
		m_UniformLocationCache.insert({ name, location });
		return location;
	}
}