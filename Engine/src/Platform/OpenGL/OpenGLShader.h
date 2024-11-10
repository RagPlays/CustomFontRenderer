#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include "Engine/Renderer/Shader.h"

namespace Engine
{
	class OpenGLShader : public Shader
	{
	public:

		explicit OpenGLShader(const std::string& filepath);
		explicit OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) const override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) const override;
		virtual void SetFloat(const std::string& name, float value) const override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) const override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) const override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) const override;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) const override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) const override;

		virtual bool UniformExists(const std::string& name) const override;

		virtual const std::string& GetName() const override;

	private:

		// Helper functions
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<unsigned int, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<unsigned int, std::string>& shaderSources);

		int GetUniformLocation(const std::string& name) const;

	private:

		uint32_t m_RendererID;
		std::string m_FilePath;
		std::string m_Name;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;

	};
}

#endif // !OPENGLSHADER_H