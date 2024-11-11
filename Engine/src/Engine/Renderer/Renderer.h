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

		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix);
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);

		static RendererAPI::API GetAPI();

	private:

		static glm::mat4 s_CurrentViewProjMatix;

	};
}

#endif // !RENDERER_H