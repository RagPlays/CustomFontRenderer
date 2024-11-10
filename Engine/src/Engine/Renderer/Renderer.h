#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>

#include "Engine/Renderer/RendererAPI.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Renderer/Camera.h"

namespace Engine
{
	class Renderer final
	{
	public:

		Renderer() = default;
		~Renderer() = default;

		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) noexcept = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other) noexcept = delete;

		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);

		static RendererAPI::API GetAPI();

	};
}

#endif // !RENDERER_H