#include "OrthographicCameraController.h"

#include <Engine.h>

using namespace Engine;

OrthographicCameraController::OrthographicCameraController(float cameraSpeed, float aspectRatio, float nearDist, float farDist, bool canRotate)
    : m_MovementSpeed{ cameraSpeed }
	, m_RotationSpeed{ 25.f }
	, m_LastMousePos{ Input::GetMousePosition() }
	, m_AspectRatio{ aspectRatio }
    , m_ZoomLevel{ 1.f }
	, m_MaxZoom{ 100.f }
	, m_MinZoom{ 0.1f }
    , m_CanRotate{ canRotate }
    , m_Camera{ nearDist, farDist }
{
	UpdateProjection();
}

void OrthographicCameraController::Update()
{
	const Timer& timer{ Timer::Get() };
	const float deltaTime{ timer.GetSeconds() };
	const float deltaSpeed{ deltaTime * m_MovementSpeed };
	const float zoomDeltaSpeed{ deltaSpeed * m_ZoomLevel };

	glm::vec3 translation{};
	glm::vec3 rotation{};

	// Forwards / Backwards
	if (Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::Up))
	{
		translation.y += zoomDeltaSpeed;
	}
	if (Input::IsKeyPressed(Key::S) || Input::IsKeyPressed(Key::Down))
	{
		translation.y -= zoomDeltaSpeed;
	}
	// Left / Right
	if (Input::IsKeyPressed(Key::A) || Input::IsKeyPressed(Key::Left))
	{
		translation.x -= zoomDeltaSpeed;
	}
	if (Input::IsKeyPressed(Key::D) || Input::IsKeyPressed(Key::Right))
	{
		translation.x += zoomDeltaSpeed;
	}
	// Sprint
	if (Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift))
	{
		translation *= 2.f;
	}

	const glm::vec2 currentMousePos{ Input::GetMousePosition() };
	if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		const float deltaX{ static_cast<float>(currentMousePos.x - m_LastMousePos.x) * m_ZoomLevel * 0.006f };
		const float deltaY{ static_cast<float>(currentMousePos.y - m_LastMousePos.y) * m_ZoomLevel * 0.006f };
		translation += glm::vec3{ -deltaX, deltaY, 0.f };
	}
	m_LastMousePos = currentMousePos;

	if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
	{
		m_ZoomLevel = 1.f;
		m_ZoomLevel = std::clamp(m_ZoomLevel, m_MinZoom, m_MaxZoom);
		UpdateProjection();
	}

    if (m_CanRotate)
    {
		const float deltaRotationSpeed{ deltaTime * m_RotationSpeed };
		if (Input::IsKeyPressed(Key::Q))
		{
			rotation += glm::vec3{0.f, 0.f, -deltaRotationSpeed };
		}
		if (Input::IsKeyPressed(Key::E))
		{
			rotation += glm::vec3{ 0.f, 0.f, -deltaRotationSpeed };
		}
    }

	m_Camera.Translate(translation);
	m_Camera.Rotate(rotation);
}

void OrthographicCameraController::OnEvent(Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
	dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

const OrthographicCamera& OrthographicCameraController::GetCamera() const
{
    return m_Camera;
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	m_ZoomLevel -= e.GetYOffset();
	m_ZoomLevel = std::clamp(m_ZoomLevel, m_MinZoom, m_MaxZoom);
	UpdateProjection();

    return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
{
	const float width{ static_cast<float>(e.GetWidth()) };
	const float height{ static_cast<float>(e.GetHeight()) };
	m_AspectRatio = width / height;

	UpdateProjection();

    return false;
}

void OrthographicCameraController::UpdateProjection()
{
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}