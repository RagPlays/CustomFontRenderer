#include "enginepch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle{ windowHandle }
	{
		ENGINE_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		const int status{ gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) };

		ENGINE_CORE_ASSERT(status, "Failed to initialize Glad!");

#if defined ENGINE_DEBUG
		ENGINE_CORE_INFO("OpenGL Info:");

		// Getting OpenGL information
		const GLubyte* vendor{ glGetString(GL_VENDOR) };
		const GLubyte* renderer{ glGetString(GL_RENDERER) };
		const GLubyte* version{ glGetString(GL_VERSION) };

		// Converting GLubyte* to std::string
		const char* vendorStr{ reinterpret_cast<const char*>(vendor) };
		const char* rendererStr{ reinterpret_cast<const char*>(renderer) };
		const char* versionStr{ reinterpret_cast<const char*>(version) };

		// Logging the information
		ENGINE_CORE_INFO("| Vendor: {0}", vendorStr);
		ENGINE_CORE_INFO("| Renderer: {0}", rendererStr);
		ENGINE_CORE_INFO("| Version: {0}", versionStr);
#endif

		ENGINE_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Engine requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}