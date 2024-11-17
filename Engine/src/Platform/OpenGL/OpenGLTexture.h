#ifndef OPENGLTEXTURE2D_H
#define OPENGLTEXTURE2D_H

#include <string>
#include <cstdint>

#include "Engine/Renderer/Texture.h"

namespace Engine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:

		explicit OpenGLTexture2D(const TextureSpecification& specification);
		explicit OpenGLTexture2D(const std::string& path, const TextureParameters& params);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;
		virtual uint32_t GetRendererID() const override;

		virtual const std::string& GetPath() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override;

		virtual bool operator==(const Texture& other) const override;

	private:

		std::string m_FilePath;
		bool m_IsLoaded;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
		unsigned int m_InternalFormat;
		unsigned int m_DataFormat;

	};
}

#endif // !OPENGLTEXTURE2D_h