#include "enginepch.h"
#include "RenderUtils2D.h"

#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Math/Math.h"

namespace Engine
{
	void RenderUtils2D::DrawBezier(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, uint32_t resolution)
	{
		glm::vec2 prevPoint{ p0 };
		const float step{ 1.f / resolution };
		float t{ step };
		for (uint32_t resIdx{}; resIdx < resolution; ++resIdx, t += step)
		{
			const glm::vec2 nextPoint{ Math::Bezier(p0, p1, p2, t) };
			Renderer2D::DrawLine(prevPoint, nextPoint);
			prevPoint = nextPoint;
		}
	}

	void RenderUtils2D::DrawBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, uint32_t resolution)
	{
		glm::vec3 prevPoint{ p0 };
		const float step{ 1.f / resolution };
		float t{ step };
		for (uint32_t resIdx{}; resIdx < resolution; ++resIdx, t += step)
		{
			const glm::vec3 nextPoint{ Math::Bezier(p0, p1, p2, t) };
			Renderer2D::DrawLine(prevPoint, nextPoint);
			prevPoint = nextPoint;
		}
	}
}