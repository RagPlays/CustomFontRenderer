#ifndef RENDERCOMMANDS_H
#define RENDERCOMMANDS_H

#include <cstdint>
#include <memory>

#include "Engine/Renderer/RendererAPI.h"

namespace Engine
{
	class RenderCommand final
	{
	public:

		RenderCommand(const RenderCommand& other) = delete;
		RenderCommand(RenderCommand&& other) noexcept = delete;
		RenderCommand& operator=(const RenderCommand& other) = delete;
		RenderCommand& operator=(RenderCommand&& other) noexcept = delete;

		static void Init();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void SetClearColor(const glm::vec4& color);
		static void Clear(bool clearColor = true, bool clearDepth = true);

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0, RenderMode mode = RenderMode::Triangle);
		static void DrawVertices(const Ref<VertexArray>& vertexArray, uint32_t vertexCount, RenderMode mode = RenderMode::Lines);

		static void SetPointSize(float size);
		static void SetLineWidth(float width);

	private:

		RenderCommand();

	private:

		static Scope<RendererAPI> s_RendererAPI;
	};
}

#endif // !RENDERCOMMANDS_H
