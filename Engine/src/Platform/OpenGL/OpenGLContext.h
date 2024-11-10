#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include "Engine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Engine
{
	class OpenGLContext : public GraphicsContext
	{
	public:

		explicit OpenGLContext(GLFWwindow* windowHandle);
		virtual ~OpenGLContext() = default;

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:

		GLFWwindow* m_WindowHandle;

	};
}

#endif // !OPENGLCONTEXT_H