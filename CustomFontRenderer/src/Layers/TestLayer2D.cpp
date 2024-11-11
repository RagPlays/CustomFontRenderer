#include "TestLayer2D.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

using namespace Engine;

TestLayer2D::TestLayer2D()
	: Layer{ "TestLayer2D" }
	, m_CameraController{ 5.f, 1.6f / 0.9f, -1.f, 1.f, true }
	, m_SquareColor{ 0.2f, 0.3f, 0.8f, 1.f }
{
}

void TestLayer2D::OnAttach()
{
	m_TestTexture = Texture2D::Create("assets/textures/transparentTexture.png");
}

void TestLayer2D::OnDetach()
{
}

void TestLayer2D::OnUpdate()
{
	Update();
	Render();
}

void TestLayer2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void TestLayer2D::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}

void TestLayer2D::Update()
{
	m_CameraController.Update();
}

void TestLayer2D::Render() const
{
	const Timer& timer{ Timer::Get() };
	const float deltaTime{ timer.GetSeconds() };
	const glm::mat4 viewProj{ m_CameraController.GetCamera().GetViewProjectionMatrix() };

	RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.f });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	{
		Renderer2D::SetDrawColor(m_SquareColor);
		Renderer2D::DrawFilledRect({ -1.f, 0.f }, { 0.8f, 0.8f });
		Renderer2D::SetDrawColor(Color::red);
		Renderer2D::DrawFilledRect({ 0.5f, -0.5f}, { 0.5f, 0.5f });
		Renderer2D::SetDrawColor(Color::darkGray);
		Renderer2D::DrawFilledRect({ 0.5f, -0.5f, -0.1f}, { 10.f, 10.f });
		Renderer2D::DrawTexture(m_TestTexture, { 0.2f, 0.5f }, { 0.5f, 0.5f }, 0.f);

	}
	Renderer2D::EndScene();
}