#include "enginepch.h"
#include "GraphicsContext.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include <GLFW/glfw3.h>
#include "Platform/OpenGL/OpenGLContext.h"
#endif

namespace Engine
{
	std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
	{
		return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
	}
}