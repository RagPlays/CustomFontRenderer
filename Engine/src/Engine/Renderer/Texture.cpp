#include "enginepch.h"
#include "Texture.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Engine
{
    Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    ENGINE_CORE_ASSERT_MSG(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(specification);
        }

        ENGINE_CORE_ASSERT_MSG(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path, const TextureParameters& params)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    ENGINE_CORE_ASSERT_MSG(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path, params);
        }

        ENGINE_CORE_ASSERT_MSG(false, "Unknown RendererAPI!");
        return nullptr;
    }
}