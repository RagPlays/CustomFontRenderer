#ifndef ORTHOGRAPHICCAMERACONTROLLER_H
#define ORTHOGRAPHICCAMERACONTROLLER_H

#include "Engine/Renderer/OrthographicCamera.h"

namespace Engine
{
	class Event;
	class MouseScrolledEvent;
	class WindowResizeEvent;
}

class OrthoCamController final
{
public:

	explicit OrthoCamController(float cameraSpeed, float nearDist, float farDist, bool canRotate);
	~OrthoCamController() = default;

	void Update();
	void OnEvent(Engine::Event& e);

	float GetZoom() const;
	void SetZoom(float newZoom);

	const Engine::OrthographicCamera& GetCamera() const;

private:

	bool OnMouseScrolled(Engine::MouseScrolledEvent& e);
	bool OnWindowResized(Engine::WindowResizeEvent& e);

	void UpdateProjection();
	void UpdateDragSpeed(float width, float height);

private:

	const float m_MovementSpeed;
	const float m_RotationSpeed;
	glm::vec2 m_LastMousePos;
	glm::vec2 m_DragSpeed;
	float m_AspectRatio;
	float m_ZoomLevel;
	const float m_MaxZoom;
	const float m_MinZoom;
	bool m_CanRotate;

	Engine::OrthographicCamera m_Camera;

};

#endif // !ORTHOGRAPHICCAMERACONTROLLER_H