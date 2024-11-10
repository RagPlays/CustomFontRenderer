#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H

#include <cstdint>
#include <memory>

#include "Engine/Renderer/BufferLayout.h"
#include "Engine/Renderer/Buffer.h"

namespace Engine
{
	class OpenGLVertexBuffer final : public VertexBuffer
	{
	public:

		explicit OpenGLVertexBuffer(uint32_t size);
		explicit OpenGLVertexBuffer(const void* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* vertices, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override;
		virtual void SetLayout(const BufferLayout& layout) override;

	private:

		uint32_t m_RendererID;
		BufferLayout m_Layout;

	};

	class OpenGLIndexBuffer final : public IndexBuffer
	{
	public:

		explicit OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override;

	private:

		uint32_t m_RendererID;
		uint32_t m_Count;

	};

}

#endif // !OPENGLBUFFER_H
