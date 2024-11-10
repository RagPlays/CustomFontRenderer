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

	void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		s_RendererAPI->DrawIndexed(vertexArray, indexCount);
	}

	void RenderCommand::DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		s_RendererAPI->DrawLines(vertexArray, vertexCount);
	}

	void RenderCommand::SetLineWidth(float width)
	{
		s_RendererAPI->SetLineWidth(width);
	}
}