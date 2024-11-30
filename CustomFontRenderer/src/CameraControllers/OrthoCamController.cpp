#include "OrthoCamController.h"

#include <Engine.h>

using namespace Engine;

OrthoCamController::OrthoCamController(float cameraSpeed, float nearDist, float farDist, bool canRotate)
    : m_MovementSpeed{ cameraSpeed }
	, m_RotationSpeed{ 25.f }
	, m_LastMousePos{ Input::GetMousePosition() }
	, m_DragSpeed{}
	, m_AspectRatio{}
    , m_ZoomLevel{ 10.f }
	, m_MaxZoom{ 1000.f }
	, m_MinZoom{ 0.1f }
    , m_CanRotate{ canRotate }
    , m_Camera{ nearDist, farDist }
{
	const Window& appWindow{ Application::Get().GetWindow() };
	const float windowWidth{ static_cast<float>(appWindow.GetWidth()) };
	const float windowHeight{ static_cast<float>(appWindow.GetHeight()) };

	m_AspectRatio = windowWidth / windowHeight;

	UpdateProjection();
	UpdateDragSpeed(windowWidth, windowHeight);
}

void OrthoCamController::Update()
{
	const FrameTimer& timer{ FrameTimer::Get() };
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

	if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
	{
		m_ZoomLevel = 1.f;
		UpdateProjection();
	}

	const glm::vec2 currentMousePos{ Input::GetMousePosition() };
	if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		const glm::vec2 mouseDelta{ (currentMousePos - m_LastMousePos) * m_ZoomLevel * m_DragSpeed };
		translation += glm::vec3{ -mouseDelta.x, mouseDelta.y, 0.f };
	}
	m_LastMousePos = currentMousePos;

    if (m_CanRotate)
    {
		const float deltaRotationSpeed{ deltaTime * m_RotationSpeed };
		if (Input::IsKeyPressed(Key::Q))
		{
			rotation += glm::vec3{ 0.f, 0.f, deltaRotationSpeed };
		}
		if (Input::IsKeyPressed(Key::E))
		{
			rotation += glm::vec3{ 0.f, 0.f, -deltaRotationSpeed };
		}
    }

	// SpeedUp
	if (Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift))
	{
		translation *= 2.f;
		rotation *= 2.f;
	}

	m_Camera.Translate(translation);
	m_Camera.Rotate(rotation);
}

void OrthoCamController::OnEvent(Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(OrthoCamController::OnMouseScrolled));
	dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(OrthoCamController::OnWindowResized));
}

float OrthoCamController::GetZoom() const
{
	return m_ZoomLevel;
}

void OrthoCamController::SetZoom(float newZoom)
{
	m_ZoomLevel = newZoom;
	UpdateProjection();
}

const OrthographicCamera& OrthoCamController::GetCamera() const
{
    return m_Camera;
}

bool OrthoCamController::OnMouseScrolled(MouseScrolledEvent& e)
{
	m_ZoomLevel -= m_ZoomLevel * e.GetYOffset() / 10.f;
	m_ZoomLevel = std::clamp(m_ZoomLevel, m_MinZoom, m_MaxZoom);

	UpdateProjection();

    return false;
}

bool OrthoCamController::OnWindowResized(WindowResizeEvent& e)
{
	// Get Window Dimentions
	const float windowWidth{ static_cast<float>(e.GetWidth()) };
	const float windowHeight{ static_cast<float>(e.GetHeight()) };

	// Set Aspect Ratio
	m_AspectRatio = windowWidth / windowHeight;

	// Adjust drag sensitivity based on the window size
	UpdateDragSpeed(windowWidth, windowHeight);

	// Update Projection
	UpdateProjection();

    return false;
}

void OrthoCamController::UpdateProjection()
{
	const float halfZoom{ m_ZoomLevel / 2.f };
	const float zoomAspect{ halfZoom * m_AspectRatio };
	m_Camera.SetProjection(-zoomAspect, zoomAspect, -halfZoom, halfZoom);
}

void OrthoCamController::UpdateDragSpeed(float width, float height)
{
	m_DragSpeed.x = 1.f / width * m_AspectRatio;
	m_DragSpeed.y = 1.f / height;
}