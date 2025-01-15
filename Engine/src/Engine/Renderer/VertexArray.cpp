#include "enginepch.h"
#include "VertexArray.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    ENGINE_CORE_ASSERT_MSG(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}

		ENGINE_CORE_ASSERT_MSG(false, "Unknown RendererAPI!");
		return nullptr;
	}
}