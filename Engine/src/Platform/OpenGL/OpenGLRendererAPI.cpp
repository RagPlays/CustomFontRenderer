#include "enginepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "Engine/Renderer/VertexArray.h"

namespace Engine
{
	void OpenGLMessageCallback
	(
		unsigned int source,
		unsigned int type,
		unsigned int id,
		unsigned int severity,
		int length,
		const char* message,
		const void* userParam
	)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         ENGINE_CORE_FATAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       ENGINE_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          ENGINE_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: ENGINE_CORE_TRACE(message); return;
		}

		ENGINE_CORE_ASSERT_MSG(false, "Unknown severity level!");
	}

	constexpr static GLenum RenderModeToOpenGL(RenderMode mode)
	{
		switch (mode)
		{
			case RenderMode::Points:			return GL_POINTS;
			case RenderMode::Lines:				return GL_LINES;
			case RenderMode::LineLoop:			return GL_LINE_LOOP;
			case RenderMode::LineStrip:			return GL_LINE_STRIP;
			case RenderMode::Triangle:			return GL_TRIANGLES;
			case RenderMode::TriangleStrip:		return GL_TRIANGLE_STRIP;
			case RenderMode::TriangleFan:		return GL_TRIANGLE_FAN;

			default:
			{
				ENGINE_CORE_ASSERT_MSG(false, "Unknown render mode!");
				return GL_NONE;
			}
		}
	}

	void OpenGLRendererAPI::Init()
	{
		ENGINE_PROFILE_FUNCTION();

#if defined ENGINE_DEBUG
		if (GLAD_GL_VERSION_4_6) ENGINE_CORE_INFO("OpenGL 4.6 is supported!");
		else ENGINE_CORE_WARN("OpenGL 4.6 or greater is not supported!");

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_TRUE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear(bool clearColor, bool clearDepth)
	{
		GLbitfield mask{};
		if (clearColor) mask |= GL_COLOR_BUFFER_BIT;
		if (clearDepth) mask |= GL_DEPTH_BUFFER_BIT;
		glClear(mask);
	}

	void OpenGLRendererAPI::DrawVertices(const Ref<VertexArray>& vertexArray, uint32_t vertexCount, RenderMode mode)
	{
		vertexArray->Bind();
		glDrawArrays(RenderModeToOpenGL(mode), 0, vertexCount); // mode, first, count
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount, RenderMode mode)
	{
		vertexArray->Bind();
		const uint32_t indicesCount{ indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount() };
		glDrawElements(RenderModeToOpenGL(mode), indicesCount, GL_UNSIGNED_INT, nullptr); // mode, count, type, *indices
	}

	void OpenGLRendererAPI::SetPointSize(float size)
	{
		glPointSize(size);
	}

	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}
}