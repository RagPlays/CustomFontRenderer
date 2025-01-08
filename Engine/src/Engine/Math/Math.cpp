#include "enginepch.h"
#include "Math.h"

namespace Engine::Math
{
	glm::vec2 Lerp(const glm::vec2& start, const glm::vec2& end, float t)
	{
		return start + (end - start) * t;
	}

	glm::vec3 Lerp(const glm::vec3& start, const glm::vec3& end, float t)
	{
		return start + (end - start) * t;
	}

	glm::vec2 Bezier(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, float t)
	{
		return Lerp(Lerp(p0, p1, t), Lerp(p1, p2, t), t);
	}

	glm::vec3 Bezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, float t)
	{
		return Lerp(Lerp(p0, p1, t), Lerp(p1, p2, t), t);
	}

	glm::vec3 Bezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t)
	{
		glm::vec3 p01 = Lerp(p0, p1, t);
		glm::vec3 p12 = Lerp(p1, p2, t);
		glm::vec3 p23 = Lerp(p2, p3, t);

		glm::vec3 p012 = Lerp(p01, p12, t);
		glm::vec3 p123 = Lerp(p12, p23, t);

		return Lerp(p012, p123, t);
	}
}