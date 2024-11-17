#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace Engine
{
	class Camera
	{
	public:

		virtual ~Camera() = default;

		const glm::vec3& GetPosition() const;
		void SetPosition(const glm::vec3& newPosition);
		void Translate(const glm::vec3& translation);

		const glm::vec3& GetRotation() const;
		void SetRotation(const glm::vec3& newRotation);
		void Rotate(const glm::vec3& rotation);

		void SetNearFarDistance(float nearDist, float farDist);

		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetProjectionMatrix() const;
		glm::mat4 GetViewProjectionMatrix() const;

		const glm::vec3& GetForward() const;
		const glm::vec3& GetRight() const;
		const glm::vec3& GetUp() const;

	protected:

		Camera();
		explicit Camera(float nearDist, float farDist);

		virtual void UpdateProjectionMatrix() = 0;

	protected:

		glm::vec3 m_Position;
		glm::vec3 m_Rotation; // (Pitch, Yaw, Roll)

		float m_Near;
		float m_Far;

		glm::vec3 m_Forward;
		glm::vec3 m_Right;
		glm::vec3 m_Up;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;

	private:

		void UpdateVectors();
		void UpdateViewMatrix();

		void NormalizeRotation();
		float NormalizeAngle(float angle);

	};
}

#endif // !CAMERA_H