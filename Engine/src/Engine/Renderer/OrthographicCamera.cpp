#include "enginepch.h"
#include "OrthographicCamera.h"

#include <glm/ext/matrix_clip_space.hpp>

namespace Engine
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearDist, float farDist)
		: Camera{ nearDist, farDist }
		, m_Left{ left }
		, m_Right{ right }
		, m_Bottom{ bottom }
		, m_Top{ top }
	{
		UpdateProjectionMatrix();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_Left = left;
		m_Right = right;
		m_Bottom = bottom;
		m_Top = top;

		UpdateProjectionMatrix();
	}

	void OrthographicCamera::UpdateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_Near, m_Far);
	}
}