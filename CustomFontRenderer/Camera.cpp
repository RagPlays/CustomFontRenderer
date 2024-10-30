#include <gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Structs.h"

Camera::Camera() // in private
	: m_CameraPos{}
    , m_CameraSpeed{ 1000.f }
    , m_ZoomSpeed{ 10 }
    , m_Near{ 0.f }
    , m_Far{ 100.f }
    , m_Zoom{ 1.f }
    , m_ViewMatrix{}
    , m_ProjectionMatrix{}
{
    UpdateProjectionMatrix();
    UpdateViewMatrix();
}

void Camera::Move(float x, float y, float z)
{
    m_CameraPos.x += x;
    m_CameraPos.y += y;
    UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
    m_CameraPos.x = x;
    m_CameraPos.y = y;
    UpdateViewMatrix();
}

const Matrix4f& Camera::GetViewMatrix() const
{
    return m_ViewMatrix;
}

const Matrix4f& Camera::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

const Point3f& Camera::GetPosition() const
{
    return m_CameraPos;
}

float Camera::GetCameraSpeed() const
{
    return m_CameraSpeed;
}

void Camera::SetZoom(float zoom)
{
    m_Zoom = zoom;
    UpdateProjectionMatrix();
}

float Camera::GetZoom() const
{
    return m_Zoom;
}

float Camera::GetZoomSpeed() const
{
    return m_ZoomSpeed;
}

void Camera::UpdateViewMatrix()
{
    m_ViewMatrix = glm::translate(Matrix4f{ 1.0f }, Point3f{ -m_CameraPos.x, -m_CameraPos.y, 0.0f });
}

void Camera::UpdateProjectionMatrix()
{
    const float left{ -400.0f * m_Zoom };
    const float right{ 400.0f * m_Zoom };
    const float bottom{ -300.0f * m_Zoom };
    const float top{ 300.0f * m_Zoom };

    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_Near, m_Far);
}