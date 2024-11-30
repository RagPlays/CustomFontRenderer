#ifndef STRUCTS_H
#define STRUCTS_H

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace Engine
{
	struct Line2i;
	struct Line2f;
	struct Line3i;
	struct Line3f;

	struct Rect2i;
	struct Rect2f;
	struct Rect3i;
	struct Rect3f;

	struct Circle2i;
	struct Circle2f;
	struct Circle3i;
	struct Circle3f;

	struct Ellipse2i;
	struct Ellipse2f;
	struct Ellipse3i;
	struct Ellipse3f;

	//////// SHAPES ////////

	// Lines //

	struct Line2i
	{
		Line2i();
		explicit Line2i(int x1, int y1, int x2, int y2);
		explicit Line2i(const glm::ivec2& pointOne, const glm::ivec2& pointTwo);

		explicit operator Line2f() const;

		glm::ivec2 pointOne;
		glm::ivec2 pointTwo;
	};

	struct Line2f
	{
		Line2f();
		explicit Line2f(float x1, float y1, float x2, float y2);
		explicit Line2f(const glm::vec2& pointOne, const glm::vec2& pointTwo);

		explicit operator Line2i() const;

		glm::vec2 pointOne;
		glm::vec2 pointTwo;
	};

	struct Line3i
	{
		Line3i();
		explicit Line3i(int x1, int y1, int z1, int x2, int y2, int z2);
		explicit Line3i(const glm::ivec3& pointOne, const glm::ivec3& pointTwo);

		explicit operator Line3f() const;

		glm::ivec3 pointOne;
		glm::ivec3 pointTwo;
	};

	struct Line3f
	{
		Line3f();
		explicit Line3f(float x1, float y1, float z1, float x2, float y2, float z2);
		explicit Line3f(const glm::vec3& pointOne, const glm::vec3& pointTwo);

		explicit operator Line3i() const;

		glm::vec3 pointOne;
		glm::vec3 pointTwo;
	};

	// Rects //

	struct Rect2i
	{
		Rect2i();
		explicit Rect2i(int x, int y, int width, int height);
		explicit Rect2i(const glm::ivec2& pos, const glm::ivec2& size);
		explicit Rect2i(const glm::ivec2& pos, int width, int height);
		explicit Rect2i(int x, int y, const glm::ivec2& size);

		explicit operator Rect2f() const;

		glm::ivec2 position;
		glm::ivec2 size;
	};

	struct Rect2f
	{
		Rect2f();
		explicit Rect2f(float x, float y, float width, float height);
		explicit Rect2f(const glm::vec2& pos, const glm::vec2& size);
		explicit Rect2f(const glm::vec2& pos, float width, float height);
		explicit Rect2f(float x, float y, const glm::vec2& size);

		explicit operator Rect2i() const;

		glm::vec2 position;
		glm::vec2 size;
	};

	struct Rect3i
	{
		Rect3i();
		explicit Rect3i(int x, int y, int z, int width, int height);
		explicit Rect3i(const glm::ivec3& pos, const glm::ivec2& size);
		explicit Rect3i(const glm::ivec3& pos, int width, int height);
		explicit Rect3i(int x, int y, int z, const glm::ivec2& size);

		explicit operator Rect3f() const;

		glm::ivec3 position;
		glm::ivec2 size;
	};

	struct Rect3f
	{
		Rect3f();
		explicit Rect3f(float x, float y, float z, float width, float height);
		explicit Rect3f(const glm::vec3& pos, const glm::vec2& size);
		explicit Rect3f(const glm::vec3& pos, float width, float height);
		explicit Rect3f(float x, float y, float z, const glm::vec2& size);

		explicit operator Rect3i() const;

		glm::vec3 position;
		glm::vec2 size;
	};

	// Cirles //

	struct Circle2i
	{
		Circle2i();
		explicit Circle2i(const glm::ivec2& center, float radius);
		explicit Circle2i(int centerX, int centerY, float radius);

		glm::ivec2 center;
		float radius;
	};

	struct Circle2f
	{
		Circle2f();
		explicit Circle2f(const glm::vec2& center, float radius);
		explicit Circle2f(float centerX, float centerY, float radius);

		glm::vec2 center;
		float radius;
	};

	struct Circle3i
	{
		Circle3i();
		explicit Circle3i(const glm::ivec3& center, float radius);
		explicit Circle3i(int centerX, int centerY, int centerZ, float radius);

		glm::ivec3 center;
		float radius;
	};

	struct Circle3f
	{
		Circle3f();
		explicit Circle3f(const glm::vec3& center, float radius);
		explicit Circle3f(float centerX, float centerY, float centerZ, float radius);

		glm::vec3 center;
		float radius;
	};

	// Ellipses //

	struct Ellipse2i
	{
		Ellipse2i();
		explicit Ellipse2i(int centerX, int centerY, int radiusX, int radiusY);
		explicit Ellipse2i(const glm::ivec2& center, int radiusX, int radiusY);

		glm::ivec2 center;
		int radiusX;
		int radiusY;
	};

	struct Ellipse2f
	{
		Ellipse2f();
		explicit Ellipse2f(float centerX, float centerY, float radiusX, float radiusY);
		explicit Ellipse2f(const glm::vec2& center, float radiusX, float radiusY);

		glm::vec2 center;
		float radiusX;
		float radiusY;
	};

	///////// OTHER	/////////
	namespace Color
	{
		static constexpr glm::vec4 red{ 1.0f, 0.0f, 0.0f, 1.f };
		static constexpr glm::vec4 orange{ 1.0f, 0.5f, 0.0f, 1.f };
		static constexpr glm::vec4 yellow{ 1.0f, 1.0f, 0.0f, 1.f };
		static constexpr glm::vec4 chartreuse{ 0.5f, 1.0f, 0.0f, 1.f };
		static constexpr glm::vec4 green{ 0.0f, 1.0f, 0.0f, 1.f };
		static constexpr glm::vec4 springGreen{ 0.0f, 1.0f, 0.5f, 1.f };
		static constexpr glm::vec4 cyan{ 0.0f, 1.0f, 1.0f, 1.f };
		static constexpr glm::vec4 azure{ 0.0f, 0.5f, 1.0f, 1.f };
		static constexpr glm::vec4 blue{ 0.0f, 0.0f, 1.0f, 1.f };
		static constexpr glm::vec4 violet{ 0.5f, 0.0f, 1.0f, 1.f };
		static constexpr glm::vec4 magenta{ 1.0f, 0.0f, 1.0f, 1.f };
		static constexpr glm::vec4 rose{ 1.0f, 0.0f, 0.5f, 1.f };
		static constexpr glm::vec4 white{ 1.0f, 1.0f, 1.0f, 1.f };
		static constexpr glm::vec4 black{ 0.0f, 0.0f, 0.0f, 1.f };
		static constexpr glm::vec4 gray{ 0.5f, 0.5f, 0.5f, 1.f };
		static constexpr glm::vec4 lightGray{ 0.75f, 0.75f, 0.75f, 1.f };
		static constexpr glm::vec4 darkGray{ 0.25f, 0.25f, 0.25f, 1.f };
	}
}

#endif // !STRUCTS_H
