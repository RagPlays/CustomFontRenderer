#include "TestLayer2D.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <Engine/Debug/Instrumentor.h>

using namespace Engine;

TestLayer2D::TestLayer2D()
	: Layer{ "TestLayer2D" }
	, m_CameraController{ 5.f, -1.f, 1.f, true }
	, m_ColorOne{ 0.8f, 0.3f, 0.2f, 1.f }
	, m_ColorTwo{ 0.2f, 0.2f, 0.2f, 1.f }
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

	Update();
	Render();
}

void TestLayer2D::OnImGuiRender()
{
	ENGINE_PROFILE_FUNCTION();

	const Camera& camera{ m_CameraController.GetCamera() };
	const glm::vec3& position{ camera.GetPosition() };
	const glm::vec3& rotation{ camera.GetRotation() };
	const float cameraZoom{ m_CameraController.GetZoom() };

	ImGui::Begin("CameraInfo");
	{
		ImGui::SetWindowFontScale(2.f);
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
		ImGui::SetWindowFontScale(2.f);
		ImGui::ColorEdit4("Color One", glm::value_ptr(m_ColorOne));
		ImGui::ColorEdit4("Color Two", glm::value_ptr(m_ColorTwo));
	}
	ImGui::End();

	// timer
	static int nrOfFrames{};
	static float elapsedTime{};
	static float lastFPS{};
	++nrOfFrames;
	elapsedTime += FrameTimer::Get().GetSeconds();
	if (elapsedTime > 1.f)
	{
		lastFPS = static_cast<float>(nrOfFrames) / elapsedTime;
		elapsedTime = 0.f;
		nrOfFrames = 0;
	}

	ImGui::Begin("RendererStats");
	{
		ImGui::SetWindowFontScale(2.f);
		ImGui::Text("General Stats:");
		ImGui::Text("FPS: %f", lastFPS);

		const auto& stats{ Renderer2D::GetStats() };
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Point Count: %d", stats.pointCount);
		ImGui::Text("Line Count: %d", stats.lineCount);
		ImGui::Text("Quad Count: %d", stats.quadCount);
		ImGui::Text("Circle Count %d", stats.circleCount);
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
			// Rects
			Renderer2D::SetDrawColor(m_ColorOne);
			Renderer2D::DrawFilledRect({ 1.f, 0.f }, { 0.8f, 0.8f }, glm::radians(-45.f));
			Renderer2D::DrawFilledRect({ -1.f, 0.f }, { 0.8f, 0.8f });
			Renderer2D::SetDrawColor(m_ColorTwo);
			Renderer2D::DrawFilledRect({ 0.5f, -0.5f }, { 0.5f, 0.75f });
			
			// Textures
			Renderer2D::DrawTexture(m_CheckerBoardTexture, { 0.f, 0.f, -0.1f }, { 20.f, 20.f }, Color::white, 10.f);
			Renderer2D::DrawTexture(m_CheckerBoardTexture, { -2.f, 0.f, 0.f }, { 1.f, 1.f }, glm::radians(rotation), Color::white, 20.f);

			// Lines
			Renderer2D::SetLineWidth(7.f);
			Renderer2D::SetDrawColor(Color::red);
			Renderer2D::DrawLine({ -10.f, 0.f, 0.2f }, { 10.f, 0.f, 0.3f });
			Renderer2D::SetDrawColor(Color::green);
			Renderer2D::DrawLine({ 0.f, -10.f, 0.2f }, { 0.f, 10.f, 0.3f });

			// Points
			Renderer2D::SetPointSize(7.f);
			Renderer2D::SetDrawColor(Color::green);
			Renderer2D::DrawPoint({ -1.f, -1.f, 0.3 });
			Renderer2D::DrawPoint({ -1.f, 1.f, 0.3 });
			Renderer2D::DrawPoint({ 1.f, -1.f, 0.3 });
			Renderer2D::DrawPoint({ 1.f, 1.f, 0.3 });
			Renderer2D::SetDrawColor(Color::cyan);
			Renderer2D::DrawPoint({ -5.f, 5.f, 0.3f });
			Renderer2D::DrawPoint({ -5.f, -5.f, 0.3f });
			Renderer2D::DrawPoint({ 5.f, 5.f, 0.3f });
			Renderer2D::DrawPoint({ 5.f, -5.f, 0.3f });
			Renderer2D::SetDrawColor(Color::red);
			Renderer2D::DrawPoint({-10.f, -10.f, 0.3f});
			Renderer2D::DrawPoint({-10.f, 10.f, 0.3f});
			Renderer2D::DrawPoint({10.f, -10.f, 0.3f});
			Renderer2D::DrawPoint({10.f, 10.f, 0.3f});

			for (float y{ -5.f }; y < 5.f; y += 0.5f)
			{
				for (float x{ -5.f }; x < 5.f; x += 0.5f)
				{
					const glm::vec4 color{ ((x + 5.0f) / 10.f), 0.5f, (y + 5.f) / 10.f, 0.5f };
					Renderer2D::SetDrawColor(color);
					Renderer2D::DrawFilledRect({ x + 0.25f, y + 0.25 }, { 0.45f, 0.45f });
				}
			}
			
			constexpr float circleThickness{ 0.125f };
			constexpr float circleRadius{ 1.25f };
			constexpr float smallCircleRadius{ 1.f };
			Renderer2D::SetDrawColor(m_ColorOne);
			Renderer2D::DrawCircle(Circle3f{ glm::vec3{ 10.f - circleRadius, 10.f - circleRadius, 0.6f }, circleRadius }, circleThickness);
			Renderer2D::DrawCircle(Circle3f{ glm::vec3{ 10.f - circleRadius, -10.f + circleRadius, 0.6f }, circleRadius }, circleThickness);
			Renderer2D::DrawCircle(Circle3f{ glm::vec3{ -10.f + circleRadius, 10.f - circleRadius, 0.6f }, circleRadius }, circleThickness);
			Renderer2D::DrawCircle(Circle3f{ glm::vec3{ -10.f + circleRadius, -10.f + circleRadius, 0.6f }, circleRadius }, circleThickness);

			Renderer2D::SetDrawColor(m_ColorTwo);
			Renderer2D::DrawFilledCircle(Circle3f{ glm::vec3{ 10.f - circleRadius, 10.f - circleRadius, 0.6f }, smallCircleRadius });
			Renderer2D::DrawFilledCircle(Circle3f{ glm::vec3{ 10.f - circleRadius, -10.f + circleRadius, 0.6f }, smallCircleRadius });
			Renderer2D::DrawFilledCircle(Circle3f{ glm::vec3{ -10.f + circleRadius, 10.f - circleRadius, 0.6f }, smallCircleRadius });
			Renderer2D::DrawFilledCircle(Circle3f{ glm::vec3{ -10.f + circleRadius, -10.f + circleRadius, 0.6f }, smallCircleRadius });
		}
		Renderer2D::EndScene();
	}
}