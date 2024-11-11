#include "enginepch.h"
#include "Renderer2D.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Engine/Renderer/Camera.h"
#include "Engine/Core/Structs.h"

#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/RendererAPI.h"

#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
	struct Renderer2DData
	{
		Ref<VertexArray> quadVertexArray{};
		Ref<Texture> whiteTexture{};
		Ref<Shader> textureShader{};

		glm::vec4 drawColor{ 1.f, 1.f, 1.f, 1.f };
		float lineWidth{ 2.0f };
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		CreateBasicResources();
	}

	void Renderer2D::Shutdown()
	{

	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data.textureShader->Bind();
		s_Data.textureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::Flush()
	{

	}

	const glm::vec4& Renderer2D::GetDrawColor()
	{
		return s_Data.drawColor;
	}

	void Renderer2D::SetDrawColor(const glm::vec4& drawColor)
	{
		s_Data.drawColor = drawColor;
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.lineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.lineWidth = width;
	}

	// Rect
	void Renderer2D::DrawFilledRect(const Rect2f& rect, float rotationAngle)
	{
		DrawFilledRect(rect.position, rect.size, rotationAngle);
	}

	void Renderer2D::DrawFilledRect(const Rect3f& rect, float rotationAngle)
	{
		DrawFilledRect(rect.position, rect.size, rotationAngle);
	}

	void Renderer2D::DrawFilledRect(const glm::vec2& position, const glm::vec2& size, float rotationAngle)
	{
		DrawFilledRect(glm::vec3{ position.x, position.y, 0.f }, size, rotationAngle);
	}

	void Renderer2D::DrawFilledRect(const glm::vec3& position, const glm::vec2& size, float rotationAngle)
	{
		const glm::mat4 transform{ glm::translate(glm::mat4{ 1.f }, position) };
		const glm::mat4 rotation{ glm::mat4_cast(glm::quat{ glm::radians(glm::vec3{ 0.f, 0.f, rotationAngle }) }) };
		const glm::mat4 scale{ glm::scale(glm::mat4{ 1.f }, glm::vec3{size.x, size.y, 1.f }) };
		const glm::mat4 modelMatrix{ transform * rotation * scale };

		s_Data.textureShader->Bind();
		s_Data.whiteTexture->Bind();
		s_Data.textureShader->SetMat4("u_ModelMatrix", modelMatrix);
		s_Data.textureShader->SetFloat4("u_TintColor", s_Data.drawColor);

		s_Data.quadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.quadVertexArray);
	}

	// Texture
	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec2& position, float rotationAngle, const glm::vec4& tintColor)
	{
		DrawTexture(texture, position, glm::vec2{texture->GetWidth(), texture->GetHeight() }, rotationAngle, tintColor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec3& position, float rotationAngle, const glm::vec4& tintColor)
	{
		DrawTexture(texture, position, glm::vec2{ texture->GetWidth(), texture->GetHeight() }, rotationAngle, tintColor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const Rect2f& rect, float rotationAngle, const glm::vec4& tintColor)
	{
		DrawTexture(texture, rect.position, rect.size, rotationAngle, tintColor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const Rect3f& rect, float rotationAngle, const glm::vec4& tintColor)
	{
		DrawTexture(texture, rect.position, rect.size, rotationAngle, tintColor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec2& position, const glm::vec2& size, float rotationAngle, const glm::vec4& tintColor)
	{
		DrawTexture(texture, glm::vec3{ position.x, position.y, 0.f }, size, rotationAngle, tintColor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec3& position, const glm::vec2& size, float rotationAngle, const glm::vec4& tintColor)
	{
		const glm::mat4 transform{ glm::translate(glm::mat4{ 1.f }, position) };
		const glm::mat4 rotation{ glm::mat4_cast(glm::quat{ glm::radians(glm::vec3{ 0.f, 0.f, rotationAngle }) }) };
		const glm::mat4 scale{ glm::scale(glm::mat4{ 1.f }, glm::vec3{size.x, size.y, 1.f }) };
		const glm::mat4 modelMatrix{ transform * rotation * scale };

		s_Data.textureShader->Bind();
		texture->Bind(0);
		s_Data.textureShader->SetMat4("u_ModelMatrix", modelMatrix);
		s_Data.textureShader->SetFloat4("u_TintColor", tintColor);

		s_Data.quadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.quadVertexArray);
	}

	void Renderer2D::CreateBasicResources()
	{
		s_Data.drawColor = Color::white;

		s_Data.quadVertexArray = VertexArray::Create();

		const std::array<float, 20> squareVertices
		{
			-0.5f, -0.5f, 0.0f, 0.f, 0.f,
			 0.5f, -0.5f, 0.0f, 1.f, 0.f,
			 0.5f,  0.5f, 0.0f, 1.f, 1.f,
			-0.5f,  0.5f, 0.0f, 0.f, 1.f
		};
		const Ref<VertexBuffer> squareVB{ VertexBuffer::Create(squareVertices.data(), sizeof(squareVertices)) };
		BufferLayout squareBufferLayout
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};
		squareVB->SetLayout(squareBufferLayout);
		s_Data.quadVertexArray->AddVertexBuffer(squareVB);

		const std::array<uint32_t, 6> squareIndices
		{
			0, 1, 2, 2, 3, 0
		};
		const Ref<IndexBuffer> squareIndexBuffer{ IndexBuffer::Create(squareIndices.data(), static_cast<uint32_t>(squareIndices.size())) };
		s_Data.quadVertexArray->SetIndexBuffer(squareIndexBuffer);

		s_Data.whiteTexture = Texture2D::Create(TextureSpecification{});
		uint32_t whiteTextureData{ 0xffffffff };
		s_Data.whiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		s_Data.textureShader = Shader::Create("assets/shaders/texture.glsl");
		s_Data.textureShader->Bind();
		s_Data.textureShader->SetInt("u_Texture", 0);
	}
}