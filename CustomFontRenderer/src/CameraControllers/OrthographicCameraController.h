#ifndef ORTHOGRAPHICCAMERACONTROLLER_H
#define ORTHOGRAPHICCAMERACONTROLLER_H

#include "Engine/Renderer/OrthographicCamera.h"

namespace Engine
{
	class Event;
	class MouseScrolledEvent;
	class WindowResizeEvent;
}

class OrthographicCameraController final
{
public:

	explicit OrthographicCameraController(float cameraSpeed, float aspectRatio, float nearDist, float farDist, bool canRotate);
	~OrthographicCameraController() = default;

	void Update();

	void OnEvent(Engine::Event& e);

	const Engine::OrthographicCamera& GetCamera() const;

private:

	bool OnMouseScrolled(Engine::MouseScrolledEvent& e);
	bool OnWindowResized(Engine::WindowResizeEvent& e);

	void UpdateProjection();

private:

	const float m_MovementSpeed;
	const float m_RotationSpeed;
	glm::vec2 m_LastMousePos;
	float m_AspectRatio;
	float m_ZoomLevel;
	const float m_MaxZoom;
	const float m_MinZoom;
	bool m_CanRotate;

	Engine::OrthographicCamera m_Camera;

};

#endif // !ORTHOGRAPHICCAMERACONTROLLER_H