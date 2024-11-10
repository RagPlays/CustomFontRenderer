#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <cstdint>
#include <memory>

#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/RendererAPI.h"

namespace Engine
{
	class Renderer2D final
	{
	public:

		Renderer2D() = default;
		~Renderer2D() = default;

		Renderer2D(const Renderer2D& other) = delete;
		Renderer2D(Renderer2D&& other) noexcept = delete;
		Renderer2D& operator=(const Renderer2D& other) = delete;
		Renderer2D& operator=(Renderer2D&& other) noexcept = delete;

		static void Init();
		static void Shutdown();

	private:

	};
}

#endif // !RENDERER2D_H