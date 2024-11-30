#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <cstdint>
#include <memory>

#include <glm/ext/vector_float4.hpp>

#include "Engine/Renderer/RendererAPI.h"

namespace Engine
{
	class VertexArray;

	class OpenGLRendererAPI : public RendererAPI
	{
	public:

		OpenGLRendererAPI() = default;
		virtual ~OpenGLRendererAPI() = default;

		OpenGLRendererAPI(const OpenGLRendererAPI& other) = delete;
		OpenGLRendererAPI(OpenGLRendererAPI&& other) noexcept = delete;
		OpenGLRendererAPI& operator=(const OpenGLRendererAPI& other) = delete;
		OpenGLRendererAPI& operator=(OpenGLRendererAPI&& other) noexcept = delete;

		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear(bool clearColor, bool clearDepth) override;

		virtual void DrawVertices(const Ref<VertexArray>& vertexArray, uint32_t vertexCount, RenderMode mode = RenderMode::Lines) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0, RenderMode mode = RenderMode::Triangle) override;

		virtual void SetPointSize(float size) override;
		virtual void SetLineWidth(float width) override;

	};
}

#endif // !OPENGLRENDEER_H