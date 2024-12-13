#include "MainLayer.h"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "Parser/Font.h"
#include "Renderer/FontRenderer.h"

using namespace Engine;

MainLayer::MainLayer()
	: Layer{ "MainLayer" }
	, m_CameraController{ 2.f, -100.f, 100.f, 1000.f, 100000.f, true }
	, m_InputPossible{ true }
	, m_CurrentFontRenderConfigs{}
{
	m_CameraController.SetZoom(5000.f);

	// Create Font
	m_Font = std::make_unique<Font>("assets/Fonts/JetBrainsMono-Bold.ttf");
	//m_Font = std::make_unique<Font>("assets/Fonts/ArialRoundedMTBold.ttf");

	// Create Font Renderer
	m_FontRenderer = std::make_unique<FontRenderer>(m_Font);
}

MainLayer::~MainLayer()
{
}

void MainLayer::OnUpdate()
{
	Update();
	Render();
}

void MainLayer::OnImGuiRender()
{
	bool hoveringImGui{ false };

	static float imGuiFontScale{ 2.f };
	ImGui::Begin("ImGuiConfigs");
	{
		if (hoveringImGui || ImGui::IsWindowHovered()) hoveringImGui = true;
		ImGui::SetWindowFontScale(imGuiFontScale);
		ImGui::Text("Font Scale");
		ImGui::DragFloat("##fontScale", &imGuiFontScale, 0.5f, 1.f, 5.f, "%.2f");
	}
	ImGui::End();

	const Camera& camera{ m_CameraController.GetCamera() };
	const glm::vec3& position{ camera.GetPosition() };
	const glm::vec3& rotation{ camera.GetRotation() };
	const float cameraZoom{ m_CameraController.GetZoom() };

	ImGui::Begin("CameraInfo");
	{
		if (hoveringImGui || ImGui::IsWindowHovered()) hoveringImGui = true;
		ImGui::SetWindowFontScale(imGuiFontScale);
		ImGui::Text("Position");
		ImGui::Text("X: %.2f Y: %.2f Z: %.2f", position.x, position.y, position.z);
		ImGui::Text("Rotation");
		ImGui::Text("Z: %.2f", rotation.x, rotation.y, rotation.z);
		ImGui::Text("Zoom");
		ImGui::Text("%.2f", cameraZoom);
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
		if (hoveringImGui || ImGui::IsWindowHovered()) hoveringImGui = true;
		ImGui::SetWindowFontScale(imGuiFontScale);
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

	ImGui::Begin("FontRendererConfig");
	{
		if (hoveringImGui || ImGui::IsWindowHovered()) hoveringImGui = true;
		ImGui::SetWindowFontScale(imGuiFontScale);
		ImVec2 configWindowRes{ ImGui::GetContentRegionAvail() };

		ImGui::Text("Input Text");
		ImGui::InputTextMultiline("##inputText", &m_CurrentFontRenderConfigs.inputText, ImVec2{ configWindowRes.x, 80.f });

		ImGui::Text("DEBUG ----------------");

		ImGui::Text("Draw Lines");
		ImGui::Checkbox("##drawDebugLines", &m_CurrentFontRenderConfigs.drawDebugLines);

		ImGui::Text("Draw Points");
		ImGui::Checkbox("##drawDebugPoints", &m_CurrentFontRenderConfigs.drawDebugPoints);

		ImGui::Text("Draw Bounding Box");
		ImGui::Checkbox("##drawDedugBoundingBox", &m_CurrentFontRenderConfigs.drawDebugBoundBox);

		ImGui::Text("----------------------");
		if (ImGui::Button("Apply", ImVec2{ configWindowRes.x, 0.f }))
		{
			m_FontRenderer->SetRenderConfigs(m_CurrentFontRenderConfigs);
		}
	}
	ImGui::End();

	ImGuiIO& io{ ImGui::GetIO() };
	bool anyInteraction{ io.WantCaptureKeyboard || io.WantTextInput || io.MouseDrawCursor };
	m_InputPossible = !anyInteraction && !hoveringImGui;
}

void MainLayer::OnEvent(Engine::Event& e)
{
	if (m_InputPossible)
	{
		m_CameraController.OnEvent(e);
	}
}

void MainLayer::Update()
{
	if (m_InputPossible)
	{
		m_CameraController.Update();
	}
}

void MainLayer::Render() const
{
	Renderer2D::ResetStats();
	RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.f });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	{
		m_FontRenderer->Render();
	}
	Renderer2D::EndScene();
}