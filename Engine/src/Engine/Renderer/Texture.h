#ifndef TEXTURE_H
#define TEXTURE_H

namespace Engine
{
	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	enum class TextureFilter
	{
		Nearest,  // Closest pixel sampling
		Linear    // Linear interpolation
	};

	enum class TextureWrap
	{
		Repeat,         // Repeat the texture
		ClampToEdge,    // Clamp to edge of the texture
		ClampToBorder,  // Clamp to a border color
		MirroredRepeat  // Mirror and repeat
	};

	struct TextureParameters
	{
		TextureParameters() = default;
		explicit TextureParameters(TextureFilter filter, TextureWrap wrap)
			: minFilter{ filter }
			, magFilter{ filter }
			, wrapS{ wrap }
			, wrapT{ wrap }
		{
		}
		explicit TextureParameters(TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrapS, TextureWrap wrapT)
			: minFilter{ minFilter }
			, magFilter{ magFilter }
			, wrapS{ wrapS }
			, wrapT{ wrapT }
		{
		}

		TextureFilter minFilter{ TextureFilter::Linear };  // Default to linear filtering
		TextureFilter magFilter{ TextureFilter::Linear };
		TextureWrap wrapS{ TextureWrap::Repeat };         // Default to repeating
		TextureWrap wrapT{ TextureWrap::Repeat };
	};

	struct TextureSpecification
	{
		uint32_t width{ 1 };
		uint32_t height{ 1 };
		ImageFormat format{ ImageFormat::RGBA8 };
		TextureParameters parameters{};
	};

	class Texture
	{
	public:

		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual const std::string& GetPath() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;

	};

	class Texture2D : public Texture
	{
	public:

		virtual ~Texture2D() = default;

		static Ref<Texture2D> Create(const TextureSpecification& specification);
		static Ref<Texture2D> Create(const std::string& path, const TextureParameters& params = TextureParameters{});

	};

}

#endif // !TEXTURE_H