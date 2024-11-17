#include "enginepch.h"
#include "PerspectiveCamera.h"

#include <glm/ext/matrix_clip_space.hpp>

namespace Engine
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float nearDist, float farDist)
		: Camera{ nearDist, farDist }
	{
		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::SetFov(float newFov)
	{
		m_FOV = newFov;
		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::SetAspectRatio(float newAspect)
	{
		m_AspectRatio = newAspect;
		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::UpdateProjectionMatrix()
	{
		ENGINE_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
		//m_ProjectionMatrix[1][1] *= -1;
	}
}