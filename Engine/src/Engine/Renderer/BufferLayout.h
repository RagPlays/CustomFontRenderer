#ifndef BUFFERLAYOUT_H
#define BUFFERLAYOUT_H

#include <string>
#include <cstdint>
#include <vector>

namespace Engine
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
			default:
			{
				ENGINE_CORE_ASSERT_MSG(false, "Unknown ShaderDataType!");
				return 0;
			}
		}
	}

	struct BufferElement
	{
		ShaderDataType type;
		std::string name;
		uint32_t size;
		size_t offset;
		bool normalized;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: name{ name }
			, type{ type }
			, size{ ShaderDataTypeSize(type) }
			, offset{ 0 }
			, normalized{ normalized }
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3; // 3* float3
				case ShaderDataType::Mat4:    return 4; // 4* float4
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
				default:
				{
					ENGINE_CORE_ASSERT_MSG(false, "Unknown ShaderDataType!");
					return 0;
				}
			}
		}
	};

	class BufferLayout final
	{
	public:

		BufferLayout();
		BufferLayout(const std::initializer_list<BufferElement>& elements);

		// Getters
		uint32_t GetStride() const;
		const std::vector<BufferElement>& GetElements() const;

		// Iterators "overloading"
		std::vector<BufferElement>::iterator begin();
		std::vector<BufferElement>::iterator end();
		std::vector<BufferElement>::const_iterator begin() const;
		std::vector<BufferElement>::const_iterator end() const;

	private:

		void CalculateOffsetsAndStride();

	private:

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;

	};
}

#endif // !BUFFERLAYOUT_H
