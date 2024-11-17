#include "TestLayer2D.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <Engine/Debug/Instrumentor.h>

using namespace Engine;

TestLayer2D::TestLayer2D()
	: Layer{ "TestLayer2D" }
	, m_CameraController{ 5.f, 1.6f / 0.9f, -1.f, 1.f, true }
	, m_SquareOneColor{ 0.2f, 0.3f, 0.8f, 1.f }
	, m_SquareTwoColor{ 0.8f, 0.3f, 0.2f, 1.f }
	, nrOfFrames{}
	, elapsedTime{}
{
}

void TestLayer2D::OnAttach()
{
	ENGINE_PROFILE_FUNCTION();

	const TextureParameters texParams{ TextureFilter::Nearest, TextureWrap::Repeat };

	m_CheckerBoardTexture = Texture2D::Create("assets/textures/checkerboard.png", texParams);
	m_TestTexOne = Texture2D::Create("assets/textures/testTexture1.png", texParams);
	m_TestTexTwo = Texture2D::Create("assets/textures/testTexture1.png", texParams);
	m_TestTexThree = Texture2D::Create("assets/textures/testTexture2.png", texParams);
	m_TestTexFour = Texture2D::Create("assets/textures/testTexture3.png", texParams);
}

void TestLayer2D::OnDetach()
{
}

void TestLayer2D::OnUpdate()
{
	ENGINE_PROFILE_FUNCTION();

	// timer
	++nrOfFrames;
	elapsedTime += FrameTimer::Get().GetSeconds();
	if (elapsedTime > 1.f)
	{
		ENGINE_CORE_INFO("FPS: {0}", nrOfFrames / 1.f);
		elapsedTime -= 1.f;
		nrOfFrames = 0;
	}

	Update();
	Render();
}

void TestLayer2D::OnImGuiRender()
{
	ENGINE_PROFILE_FUNCTION();

	const Camera& camera{ m_CameraController.GetCamera() };
	const glm::vec3& position{ camera.GetPosition() };
	const glm::vec3& rotation{ camera.GetRotation() };
	float cameraZoom{ m_CameraController.GetZoom() };

	ImGui::Begin("CameraInfo");
	{
		ImGui::Text("Position");
		ImGui::Text("X: %.2f Y: %.2f Z: %.2f", position.x, position.y, position.z);
		ImGui::Text("Rotation");
		ImGui::Text("Z: %.2f", rotation.x, rotation.y, rotation.z);
		ImGui::Text("Zoom");
		ImGui::Text("%.2f", cameraZoom);
	}
	ImGui::End();

	ImGui::Begin("SceneSettings");
	{
		ImGui::ColorEdit4("Square One Color", glm::value_ptr(m_SquareOneColor));
		ImGui::ColorEdit4("Square Two Color", glm::value_ptr(m_SquareTwoColor));
		ImGui::DragFloat("RotationZ", &rotationZ, 1.0f, -180.0f, 180.0f, "%.1f");
	}
	ImGui::End();

	ImGui::Begin("RendererStats");
	{
		const auto& stats{ Renderer2D::GetStats() };
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quad Count: %d", stats.quadCount);
		ImGui::Text("Vertices Count: %d", stats.GetTotalVerticesCount());
		ImGui::Text("Indices Count: %d", stats.GetTotalIndicesCount());
	}
	ImGui::End();
}

void TestLayer2D::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}

void TestLayer2D::Update()
{
	ENGINE_PROFILE_FUNCTION();

	m_CameraController.Update();
}

void TestLayer2D::Render() const
{
	ENGINE_PROFILE_FUNCTION();

	const FrameTimer& timer{ FrameTimer::Get() };
	const float deltaTime{ timer.GetSeconds() };

	Renderer2D::ResetStats();
	{
		ENGINE_PROFILE_SCOPE("TestLayer2D::Render-Clearing");
		RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.f });
		RenderCommand::Clear();
	}

	{
		static float rotation{ 0.f };
		rotation += deltaTime * 50.f;

		ENGINE_PROFILE_SCOPE("TestLayer2D::Render-Rendering");

		Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			Renderer2D::SetDrawColor({ 0.8f, 0.3f, 0.2f, 1.f });
			Renderer2D::DrawFilledRect({ 1.f, 0.f }, { 0.8f, 0.8f }, -45.f);
			Renderer2D::DrawFilledRect({ -1.f, 0.f }, { 0.8f, 0.8f });
			Renderer2D::SetDrawColor({ 0.2f, 0.3f, 0.8f, 1.f });
			Renderer2D::DrawFilledRect({ 0.5f, -0.5f }, { 0.5f, 0.75f });
			Renderer2D::SetDrawColor(Color::darkGray);
			Renderer2D::DrawTexture(m_CheckerBoardTexture, { 0.f, 0.f, -0.1f }, { 20.f, 20.f }, Color::white, 10.f);
			Renderer2D::DrawTexture(m_CheckerBoardTexture, { -2.f, 0.f, 0.f }, { 1.f, 1.f }, rotation, Color::white, 20.f);
		}
		Renderer2D::EndScene();

		Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			for (float y{ -5.f }; y < 5.f; y += 0.1f)
			{
				for (float x{ -5.f }; x < 5.f; x += 0.1f)
				{
					const glm::vec4 color{ ((x + 5.0f) / 10.f), 0.5f, (y + 5.f) / 10.f, 0.5f };
					Renderer2D::SetDrawColor(color);
					Renderer2D::DrawFilledRect({ x, y }, { 0.45f, 0.45f });
				}
			}
		}
		Renderer2D::EndScene();
	}
}