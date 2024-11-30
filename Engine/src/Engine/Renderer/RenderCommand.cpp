#include "enginepch.h"
#include "RenderCommand.h"

namespace Engine
{
	std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI{ RendererAPI::Create() };

	void RenderCommand::Init()
	{
		s_RendererAPI->Init();
	}

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_RendererAPI->SetViewport(x, y, width, height);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		s_RendererAPI->SetClearColor(color);
	}

	void RenderCommand::Clear(bool clearColor, bool clearDepth)
	{
		s_RendererAPI->Clear(clearColor, clearDepth);
	}

	void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount, RenderMode mode)
	{
		s_RendererAPI->DrawIndexed(vertexArray, indexCount, mode);
	}

	void RenderCommand::DrawVertices(const Ref<VertexArray>& vertexArray, uint32_t vertexCount, RenderMode mode)
	{
		s_RendererAPI->DrawVertices(vertexArray, vertexCount, mode);
	}

	void RenderCommand::SetPointSize(float size)
	{
		s_RendererAPI->SetPointSize(size);
	}

	void RenderCommand::SetLineWidth(float width)
	{
		s_RendererAPI->SetLineWidth(width);
	}
}