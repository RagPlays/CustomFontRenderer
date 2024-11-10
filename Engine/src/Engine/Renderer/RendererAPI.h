#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include <cstdint>
#include <memory>

#include <glm/ext/vector_float4.hpp>

#include "Engine/Renderer/VertexArray.h"

namespace Engine
{
	class RendererAPI
	{
	public:

		enum class API
		{
			None = 0, OpenGL = 1 // Add more api's here
		};

	public:

		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear(bool clearColor, bool clearDepth) = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

		virtual void SetLineWidth(float width) = 0;

		static API GetAPI();
		static std::unique_ptr<RendererAPI> Create();

	private:

		static API s_API;

	};
}

#endif // !RENDERERAPI_H