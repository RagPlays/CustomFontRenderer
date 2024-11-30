#include "enginepch.h"
#include "Structs.h"

namespace Engine
{
	// LINES //

	Line2i::Line2i()
		: Line2i{ 0, 0, 0, 0 }
	{
	}

	Line2i::Line2i(int x1, int y1, int x2, int y2)
		: pointOne{ x1, y1 }
		, pointTwo{ x2, y2 }
	{
	}

	Line2i::Line2i(const glm::ivec2& pointOne, const glm::ivec2& pointTwo)
		: pointOne{ pointOne }
		, pointTwo{ pointTwo }
	{
	}

	Line2i::operator Line2f() const
	{
		return Line2f
		{
			glm::vec2(pointOne),
			glm::vec2(pointTwo),
		};
	}

	//

	Line2f::Line2f()
		: Line2f{ 0.f, 0.f, 0.f, 0.f }
	{
	}

	Line2f::Line2f(float x1, float y1, float x2, float y2)
		: pointOne{ x1, y1 }
		, pointTwo{ x2, y2 }
	{
	}

	Line2f::Line2f(const glm::vec2& pointOne, const glm::vec2& pointTwo)
		: pointOne{ pointOne }
		, pointTwo{ pointTwo }
	{
	}

	Line2f::operator Line2i() const
	{
		return Line2i
		{
			glm::ivec2(pointOne),
			glm::ivec2(pointTwo),
		};
	}

	//

	Line3i::Line3i()
		: pointOne{}
		, pointTwo{}
	{
	}

	Line3i::Line3i(int x1, int y1, int z1, int x2, int y2, int z2)
		: pointOne{}
		, pointTwo{}
	{
	}

	Line3i::Line3i(const glm::ivec3& pointOne, const glm::ivec3& pointTwo)
		: pointOne{}
		, pointTwo{}
	{
	}

	Line3i::operator Line3f() const
	{
		return Line3f
		{
			static_cast<glm::vec3>(pointOne),
			static_cast<glm::vec3>(pointTwo)
		};
	}

	//

	Line3f::Line3f()
		: pointOne{}
		, pointTwo{}
	{
	}

	Line3f::Line3f(float x1, float y1, float z1, float x2, float y2, float z2)
		: pointOne{ x1, y1, z1 }
		, pointTwo{ x2, y2, z2 }
	{
	}

	Line3f::Line3f(const glm::vec3& pointOne, const glm::vec3& pointTwo)
		: pointOne{ pointOne }
		, pointTwo{ pointTwo }
	{
	}

	Line3f::operator Line3i() const
	{
		return Line3i
		{
			static_cast<glm::ivec3>(pointOne),
			static_cast<glm::ivec3>(pointTwo)
		};
	}

	// RECTS //

	Rect2i::Rect2i()
		: position{ 0, 0 }
		, size{ 0, 0 }
	{
	}

	Rect2i::Rect2i(int x, int y, int width, int height)
		: position{ x, y }
		, size{ width, height }
	{
	}

	Rect2i::Rect2i(const glm::ivec2& pos, const glm::ivec2& size)
		: position{ pos }
		, size{ size }
	{
	}

	Rect2i::Rect2i(const glm::ivec2& pos, int width, int height)
		: position{ pos }
		, size{ width, height }
	{
	}

	Rect2i::Rect2i(int x, int y, const glm::ivec2& size)
		: position{ x, y }
		, size{ size }
	{
	}

	Rect2i::operator Rect2f() const
	{
		return Rect2f
		{
			static_cast<glm::vec2>(position),
			static_cast<glm::vec2>(size)
		};
	}

	//

	Rect2f::Rect2f()
		: position{}
		, size{}
	{
	}

	Rect2f::Rect2f(float x, float y, float width, float height)
		: position{ x, y }
		, size{ width, height }
	{
	}

	Rect2f::Rect2f(const glm::vec2& pos, const glm::vec2& size)
		: position{ pos.x, pos.y }
		, size{ size }
	{
	}

	Rect2f::Rect2f(const glm::vec2& pos, float width, float height)
		: position{ pos.x, pos.y }
		, size{ width, height }
	{
	}

	Rect2f::Rect2f(float x, float y, const glm::vec2& size)
		: position{ x, y }
		, size{ size }
	{
	}

	Rect2f::operator Rect2i() const
	{
		return Rect2i
		{
			static_cast<glm::ivec2>(position),
			static_cast<glm::ivec2>(size)
		};
	}

	//

	Rect3i::Rect3i()
		: position{}
		, size{}
	{
	}

	Rect3i::Rect3i(int x, int y, int z, int width, int height)
		: position{ x, y, z }
		, size{ width, height }
	{
	}

	Rect3i::Rect3i(const glm::ivec3& pos, const glm::ivec2& size)
		: position{ pos }
		, size{ size }
	{
	}

	Rect3i::Rect3i(const glm::ivec3& pos, int width, int height)
		: position{ pos }
		, size{ width, height }
	{
	}

	Rect3i::Rect3i(int x, int y, int z, const glm::ivec2& size)
		: position{ x, y, z }
		, size{ size }
	{
	}

	Rect3i::operator Rect3f() const
	{
		return Rect3f
		{
			static_cast<glm::vec3>(position),
			static_cast<glm::vec2>(size)
		};
	}

	//

	Rect3f::Rect3f()
		: position{}
		, size{}
	{
	}

	Rect3f::Rect3f(float x, float y, float z, float width, float height)
		: position{ x, y, z }
		, size{ width, height }
	{
	}

	Rect3f::Rect3f(const glm::vec3& pos, const glm::vec2& size)
		: position{ pos }
		, size{ size }
	{
	}

	Rect3f::Rect3f(const glm::vec3& pos, float width, float height)
		: position{ pos }
		, size{ width, height }
	{
	}

	Rect3f::Rect3f(float x, float y, float z, const glm::vec2& size)
		: position{ x, y, z }
		, size{ size }
	{
	}

	Rect3f::operator Rect3i() const
	{
		return Rect3i
		{
			static_cast<glm::ivec3>(position),
			static_cast<glm::ivec2>(size)
		};
	}

	// CIRCLEI / CIRCLEF //

	Circle2i::Circle2i()
		: Circle2i{ 0, 0, 0 }
	{
	}

	Circle2i::Circle2i(const glm::ivec2& center, float radius)
		: center{ center }
		, radius{ radius }
	{
	}

	Circle2i::Circle2i(int centerX, int centerY, float radius)
		: center{ centerX, centerY }
		, radius{ radius }
	{
	}

	//

	Circle2f::Circle2f()
		: Circle2f{ 0.f, 0.f, 0.f }
	{
	}

	Circle2f::Circle2f(const glm::vec2& center, float radius)
		: center{ center }
		, radius{ radius }
	{
	}

	Circle2f::Circle2f(float centerX, float centerY, float radius)
		: center{ centerX, centerY }
		, radius{ radius }
	{
	}

	//

	Circle3i::Circle3i()
		: center{ 0, 0, 0 }
		, radius{ 0.f }
	{
	}

	Circle3i::Circle3i(const glm::ivec3& center, float radius)
		: center{ center }
		, radius{ radius }
	{
	}

	Circle3i::Circle3i(int centerX, int centerY, int centerZ, float radius)
		: center{ centerX, centerY, centerZ }
		, radius{ radius }
	{
	}

	//

	Circle3f::Circle3f()
		: center{ 0.f, 0.f, 0.f }
		, radius{ 0.f }
	{
	}

	Circle3f::Circle3f(const glm::vec3& center, float radius)
		: center{ center }
		, radius{ radius }
	{
	}

	Circle3f::Circle3f(float centerX, float centerY, float centerZ, float radius)
		: center{ centerX, centerY, centerZ }
		, radius{ radius }
	{
	}

	// ELLIPSEI / ELLIPSEF //

	Ellipse2i::Ellipse2i()
		: Ellipse2i{ 0, 0, 0, 0 }
	{
	}

	Ellipse2i::Ellipse2i(int centerX, int centerY, int radiusX, int radiusY)
		: center{ centerX, centerY }
		, radiusX{ radiusX }
		, radiusY{ radiusY }
	{
	}

	Ellipse2i::Ellipse2i(const glm::ivec2& center, int radiusX, int radiusY)
		: center{ center }
		, radiusX{ radiusX }
		, radiusY{ radiusY }
	{
	}

	Ellipse2f::Ellipse2f()
		: Ellipse2f{ 0.f, 0.f, 0.f, 0.f }
	{
	}

	Ellipse2f::Ellipse2f(float centerX, float centerY, float radiusX, float radiusY)
		: center{ centerX, centerY }
		, radiusX{ radiusX }
		, radiusY{ radiusY }
	{
	}

	Ellipse2f::Ellipse2f(const glm::vec2& center, float radiusX, float radiusY)
		: center{ center }
		, radiusX{ radiusX }
		, radiusY{ radiusY }
	{
	}

}