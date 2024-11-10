#include "enginepch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Engine
{
	static GLenum ImageFormatToGLDataFormat(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RGB8:  return GL_RGB;
		case ImageFormat::RGBA8: return GL_RGBA;
		}

		ENGINE_CORE_ASSERT(false, "Invalid format");
		return 0;
	}

	static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RGB8:  return GL_RGB8;
		case ImageFormat::RGBA8: return GL_RGBA8;
		}

		ENGINE_CORE_ASSERT(false, "Invalid format");
		return 0;
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification{ specification }
		, m_IsLoaded{ false }
		, m_Width{ specification.width }
		, m_Height{ specification.height }
		, m_RendererID{}
		, m_InternalFormat{ ImageFormatToGLInternalFormat(specification.format) }
		, m_DataFormat{ ImageFormatToGLDataFormat(specification.format) }
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Specification{}
		, m_IsLoaded{}
		, m_Width{}
		, m_Height{}
		, m_RendererID{}
		, m_InternalFormat{}
		, m_DataFormat{}
	{
		int width{};
		int height{};
		int channels{};

		stbi_set_flip_vertically_on_load(1); // needs to be flipped due to opengl want bottom to top and stb load top to bottom

		stbi_uc* data{ stbi_load(path.c_str(), &width, &height, &channels, 0) };
		ENGINE_CORE_ASSERT(data, "Failed to load image!");

		if (data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;

			unsigned int internalFormat{};
			unsigned int dataFormat{};

			switch (channels)
			{
			case 3:
				{
					internalFormat = GL_RGB8;
					dataFormat = GL_RGB;
					break;
				}
			case 4:
				{
					internalFormat = GL_RGBA8;
					dataFormat = GL_RGBA;
					break;
				}
			default:
				{
					ENGINE_CORE_ASSERT(false, "Format not supported");
					break;
				}
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	const TextureSpecification& OpenGLTexture2D::GetSpecification() const
	{
		return m_Specification;
	}

	uint32_t OpenGLTexture2D::GetWidth() const
	{
		return m_Width;
	}

	uint32_t OpenGLTexture2D::GetHeight() const
	{
		return m_Height;
	}

	uint32_t OpenGLTexture2D::GetRendererID() const
	{
		return m_RendererID;
	}

	const std::string& OpenGLTexture2D::GetPath() const
	{
		return m_FilePath;
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		const uint32_t bpp{ static_cast<uint32_t>(m_DataFormat == GL_RGBA ? 4 : 3) };
		ENGINE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	bool OpenGLTexture2D::IsLoaded() const
	{
		return m_IsLoaded;
	}

	bool OpenGLTexture2D::operator==(const Texture& other) const
	{
		return m_RendererID == other.GetRendererID();
	}
}