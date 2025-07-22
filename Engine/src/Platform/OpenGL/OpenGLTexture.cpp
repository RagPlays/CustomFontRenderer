#include "enginepch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Engine
{
	static constexpr GLenum ImageFormatToGLDataFormat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::RGB8:  return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;
			default:
			{
				ENGINE_CORE_ASSERT_MSG(false, "Invalid format");
				return GL_NONE;
			}
		}
	}

	static constexpr GLenum ImageFormatToGLInternalFormat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::RGB8:  return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			default:
			{
				ENGINE_CORE_ASSERT_MSG(false, "Invalid format");
				return GL_NONE;
			}
		}
	}

	static constexpr GLenum ToOpenGLFilter(TextureFilter filter)
	{
		switch (filter)
		{
			case TextureFilter::Nearest: return GL_NEAREST;
			case TextureFilter::Linear:  return GL_LINEAR;
			default:
			{
				ENGINE_CORE_ASSERT_MSG(false, "Unknown TextureFilter!");
				return GL_NONE;
			}
		}
	}

	static constexpr GLenum ToOpenGLWrap(TextureWrap wrap)
	{
		switch (wrap)
		{
			case TextureWrap::Repeat:         return GL_REPEAT;
			case TextureWrap::ClampToEdge:    return GL_CLAMP_TO_EDGE;
			case TextureWrap::ClampToBorder:  return GL_CLAMP_TO_BORDER;
			case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
			default:
			{
				ENGINE_CORE_ASSERT_MSG(false, "Unknown TextureWrap!");
				return GL_NONE;
			}
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_IsLoaded{ false }
		, m_Width{ specification.width }
		, m_Height{ specification.height }
		, m_RendererID{}
		, m_InternalFormat{ ImageFormatToGLInternalFormat(specification.format) }
		, m_DataFormat{ ImageFormatToGLDataFormat(specification.format) }
	{
		ENGINE_PROFILE_FUNCTION();

		ENGINE_CORE_ASSERT_MSG(m_InternalFormat != GL_NONE && m_DataFormat != GL_NONE, "Invalid texture formats!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		const TextureParameters& params{ specification.parameters };
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, ToOpenGLFilter(params.minFilter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, ToOpenGLFilter(params.magFilter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, ToOpenGLWrap(params.wrapS));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, ToOpenGLWrap(params.wrapT));
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const TextureParameters& params)
		: m_IsLoaded{}
		, m_Width{}
		, m_Height{}
		, m_RendererID{}
		, m_InternalFormat{}
		, m_DataFormat{}
	{
		ENGINE_PROFILE_FUNCTION();

		int width{};
		int height{};
		int channels{};

		stbi_set_flip_vertically_on_load(1); // flip vertically to match OpenGL's convention

		stbi_uc* data{ nullptr };
		{
			ENGINE_PROFILE_SCOPE("OpenGLTexture2D::OpenGLTexture2D-stbi_load");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (!data)
		{
			ENGINE_CORE_ERROR("Failed to load image at path: {0}", path.c_str());
			ENGINE_CORE_ASSERT_MSG(data, "Failed to load image!");
			return;
		}

		m_IsLoaded = true;

		m_Width = width;
		m_Height = height;

		// Assign internal format and data format based on channel count
		switch (channels)
		{
			case 3:
			{
				m_InternalFormat = GL_RGB8;
				m_DataFormat = GL_RGB;
				break;
			}
			case 4:
			{
				m_InternalFormat = GL_RGBA8;
				m_DataFormat = GL_RGBA;
				break;
			}
			default:
			{
				ENGINE_CORE_ERROR("Unsupported image format with {0} channels", channels);
				stbi_image_free(data);
				ENGINE_CORE_ASSERT_MSG(false, "Format not supported");
				break;
			}
		}

		ENGINE_CORE_ASSERT_MSG(m_InternalFormat != GL_NONE && m_DataFormat != GL_NONE, "Invalid texture formats!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, ToOpenGLFilter(params.minFilter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, ToOpenGLFilter(params.magFilter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, ToOpenGLWrap(params.wrapS));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, ToOpenGLWrap(params.wrapT));

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ENGINE_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
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

	void OpenGLTexture2D::SetData(const void* data, uint32_t size)
	{
		const uint32_t bpp{ static_cast<uint32_t>(m_DataFormat == GL_RGBA ? 4 : 3) }; // bytes per pixel
		ENGINE_CORE_ASSERT_MSG(size == m_Width * m_Height * bpp, "Data must be entire texture!");

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		ENGINE_PROFILE_FUNCTION();

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