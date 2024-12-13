#include "SandboxMainLayer.h"

#include <array>

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

using namespace Engine;

SandboxMainLayer::SandboxMainLayer()
	: Layer{ "SandboxMainLayer" }
	, m_CameraController{ 5.f, -1.f, 1.f, true }
	, m_SquareColor{ 0.2f, 0.2f, 0.8f, 1.f }
	, m_TriangleRotationZ{}
	//, m_TriangleModel{}
	//, m_SquareModel{}
{
	// Triangle //
	Ref<VertexArray> triangleVA{ VertexArray::Create() };

	constexpr std::array<float, 21> triangleVertices
	{
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};
	const Ref<VertexBuffer> triangleVB{ VertexBuffer::Create(triangleVertices.data(), sizeof(triangleVertices)) };
	BufferLayout triangleBufferLayout
	{
		{ ShaderDataType::Float3, "a_Position"},
		{ ShaderDataType::Float4, "a_Color" }
	};
	triangleVB->SetLayout(triangleBufferLayout);
	triangleVA->AddVertexBuffer(triangleVB);

	constexpr std::array<uint32_t, 3> triangleIndices
	{
		0, 1, 2
	};
	const Ref<IndexBuffer> triangleIB{ IndexBuffer::Create(triangleIndices.data(), static_cast<uint32_t>(triangleIndices.size())) };
	triangleVA->SetIndexBuffer(triangleIB);

	// Square //
	Ref<VertexArray> squareVA{ VertexArray::Create() };

	constexpr std::array<float, 20> squareVertices
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};
	const Ref<VertexBuffer> squareVB{ VertexBuffer::Create(squareVertices.data(), sizeof(squareVertices)) };
	BufferLayout squareBufferLayout
	{
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" }
	};
	squareVB->SetLayout(squareBufferLayout);
	squareVA->AddVertexBuffer(squareVB);

	constexpr std::array<uint32_t, 6> squareIndices
	{
		0, 1, 2, 2, 3, 0
	};
	const Ref<IndexBuffer> squareIndexBuffer{ IndexBuffer::Create(squareIndices.data(), static_cast<uint32_t>(squareIndices.size())) };
	squareVA->SetIndexBuffer(squareIndexBuffer);

	// Shaders //
	m_BasicShader = Shader::Create("assets/shaders/basicShader.glsl");
	m_FlatColorShader = Shader::Create("assets/shaders/flatColor.glsl");
	m_TextureShader = Shader::Create("assets/shaders/texture.glsl");
	m_TextureShader->Bind();
	m_TextureShader->SetInt("u_Texture", 0);

	//m_TriangleModel = CreateScope<Model>(m_BasicShader, triangleVA);
	//m_SquareModel = CreateScope<Model>(m_FlatColorShader, squareVA);

	m_Texture = Texture2D::Create("assets/textures/testTexture2.png");
	m_TransparentTexture = Texture2D::Create("assets/textures/transparentTexture.png");
}

void SandboxMainLayer::OnUpdate()
{
	Update();
	Render();
}

void SandboxMainLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::InputFloat("Triangle Rotation", &m_TriangleRotationZ);
	ImGui::End();
}

void SandboxMainLayer::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}

void SandboxMainLayer::Update()
{
	m_CameraController.Update();

	const FrameTimer& timer{ FrameTimer::Get() };
	const float deltaTime{ timer.GetSeconds() };

	m_FlatColorShader->Bind();
	m_FlatColorShader->SetFloat4("u_Color", m_SquareColor);
}

void SandboxMainLayer::Render() const
{
	const FrameTimer& timer{ FrameTimer::Get() };
	const float deltaTime{ timer.GetSeconds() };
	const glm::mat4 viewProj{ m_CameraController.GetCamera().GetViewProjectionMatrix() };

	RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.f });
	RenderCommand::Clear();

	//m_TriangleModel->SetRotation(glm::vec3{ 0.f, 0.f, m_TriangleRotationZ });
	/*m_TriangleModel->Submit(viewProj);

	m_SquareModel->SetScale(glm::vec3{ 0.1f, 0.1f, 0.1f });
	m_SquareModel->SetShader(m_FlatColorShader);
	for (int y{}; y < 20; ++y)
	{
		for (int x{}; x < 19; ++x)
		{
			m_SquareModel->SetPosition(glm::vec3{ x * 0.11f, y * 0.11f, 0.0f });
			m_SquareModel->Submit(viewProj);
		}
	}

	m_SquareModel->SetShader(m_TextureShader);
	m_SquareModel->SetPosition(glm::vec3{});
	m_SquareModel->SetScale(glm::vec3{1.5f, 1.5f, 1.5f});

	m_Texture->Bind();
	m_SquareModel->Submit(viewProj);

	m_TransparentTexture->Bind();
	m_SquareModel->Submit(viewProj);*/

}