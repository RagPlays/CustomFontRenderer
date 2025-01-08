#ifndef RENDERUTILS2D_H
#define RENDERUTILS2D_H

#include "Engine/Math/Math.h"

namespace Engine
{
	class RenderUtils2D final
	{
	public:

		RenderUtils2D() = default;
		~RenderUtils2D() = default;

		static void DrawBezier(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, uint32_t resolution);
		static void DrawBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, uint32_t resolution);

	};
}

#endif // !RENDERUTILS2D_H