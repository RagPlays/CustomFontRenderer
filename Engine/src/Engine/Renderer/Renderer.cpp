#include "enginepch.h"
#include "Renderer.h"

#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Renderer2D.h"

namespace Engine
{
    glm::mat4 Renderer::s_CurrentViewProjMatix{ 1.f };

    void Renderer::Init()
    {
        ENGINE_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        ENGINE_PROFILE_FUNCTION();

        Renderer2D::Shutdown();
    }

    void Renderer::BeginScene(const Camera& camera)
    {
        s_CurrentViewProjMatix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix)
    {
        if (shader->UniformExists("u_ViewProjectionMatrix") && shader->UniformExists("u_ModelMatrix"))
        {
            shader->Bind();
            shader->SetMat4("u_ViewProjectionMatrix", s_CurrentViewProjMatix);
            shader->SetMat4("u_ModelMatrix", modelMatrix);
            RenderCommand::DrawIndexed(vertexArray);
        }
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
    {
        shader->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    RendererAPI::API Renderer::GetAPI()
    {
        return RendererAPI::GetAPI();
    }
}