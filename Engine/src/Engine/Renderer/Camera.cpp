#include "enginepch.h"
#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine
{
	// Public //
	void Camera::SetPosition(const glm::vec3& newPosition)
	{
		m_Position = newPosition;

		UpdateViewMatrix();
	}

	void Camera::Translate(const glm::vec3& offset)
	{
		m_Position += offset;

		UpdateViewMatrix();
	}

	void Camera::SetRotation(const glm::vec3& newRotation)
	{
		m_Rotation = newRotation;

		UpdateVectors();
		UpdateViewMatrix();
	}

	void Camera::Rotate(const glm::vec3& offset)
	{
		m_Rotation += offset;

		UpdateVectors();
		UpdateViewMatrix();
	}

	void Camera::SetNearFarDistance(float nearDist, float farDist)
	{
		m_Near = nearDist;
		m_Far = farDist;
		UpdateProjectionMatrix();
	}

	const glm::mat4& Camera::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	const glm::mat4& Camera::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	glm::mat4 Camera::GetViewProjectionMatrix() const
	{
		return m_ProjectionMatrix * m_ViewMatrix;
	}

	const glm::vec3& Camera::GetForward() const
	{
		return m_Forward;
	}

	const glm::vec3& Camera::GetRight() const
	{
		return m_Right;
	}

	const glm::vec3& Camera::GetUp() const
	{
		return m_Up;
	}

	// Protected //
	Camera::Camera()
		: Camera{ -1.f, 1.f }
	{
	}

	Camera::Camera(float nearDist, float farDist)
		: m_Position{}
		, m_Rotation{}
		, m_Near{ nearDist }
		, m_Far{ farDist }
		, m_Forward{}
		, m_Right{}
		, m_Up{}
		, m_ViewMatrix{ glm::mat4{ 1.f } }
		, m_ProjectionMatrix{ glm::mat4{ 1.f } }
	{
		UpdateVectors();
		UpdateViewMatrix();
	}

	// Private //
	void Camera::UpdateVectors()
	{
		constexpr glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };

		// Forward
		m_Forward.x = std::cos(glm::radians(m_Rotation.y)) * std::cos(glm::radians(m_Rotation.x));
		m_Forward.y = std::sin(glm::radians(m_Rotation.x));
		m_Forward.z = std::sin(glm::radians(m_Rotation.y)) * std::cos(glm::radians(m_Rotation.x));
		m_Forward = glm::normalize(m_Forward);

		// Right
		m_Right = glm::normalize(glm::cross(m_Forward, worldUp));

		// Up
		m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
	}

	void Camera::UpdateViewMatrix()
	{
		const glm::mat4 rotation{ glm::mat4_cast(glm::quat{ glm::radians(m_Rotation) }) };
		const glm::mat4 transform{ glm::translate(glm::mat4{ 1.f }, m_Position) };

		m_ViewMatrix = glm::inverse(transform * rotation);
	}
}