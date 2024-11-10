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

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
		static void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount);

		static void SetLineWidth(float width);

	private:

		RenderCommand();

	private:

		static Scope<RendererAPI> s_RendererAPI;
	};
}

#endif // !RENDERCOMMANDS_H
