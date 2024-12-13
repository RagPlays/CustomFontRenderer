#include "enginepch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Engine
{
	RendererAPI::API RendererAPI::s_API{ RendererAPI::API::OpenGL }; // Set Render API here

	RendererAPI::API RendererAPI::GetAPI()
	{
		return s_API;
	}

	std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    ENGINE_CORE_ASSERT_MSG(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}

		ENGINE_CORE_ASSERT_MSG(false, "Unknown RendererAPI!");
		return nullptr;
	}
}