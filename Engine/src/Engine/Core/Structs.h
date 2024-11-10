#ifndef STRUCTS_H
#define STRUCTS_H

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace Engine
{
	struct Linef;
	struct Linei;
	struct Rectf;
	struct Recti;
	struct Circlef;
	struct Circlei;
	struct Ellipsei;
	struct Ellipsef;
	struct Color4f;

	//////// SHAPES ////////

	struct Linei
	{
		Linei();
		explicit Linei(int x1, int y1, int x2, int y2);
		explicit Linei(const glm::ivec2& pointOne, const glm::ivec2& pointTwo);

		explicit operator Linef() const;

		glm::ivec2 pointOne;
		glm::ivec2 pointTwo;
	};

	struct Linef
	{
		Linef();
		explicit Linef(float x1, float y1, float x2, float y2);
		explicit Linef(const glm::vec2& pointOne, const glm::vec2& pointTwo);

		explicit operator Linei() const;

		glm::vec2 pointOne;
		glm::vec2 pointTwo;
	};

	struct Recti
	{
		Recti();
		explicit Recti(int x, int y, int width, int height);
		explicit Recti(const glm::ivec2& pos, const glm::ivec2& size);
		explicit Recti(const glm::ivec2& pos, int width, int height);

		explicit operator Rectf() const;

		int x;
		int y;
		int width;
		int height;
	};

	struct Rectf
	{
		Rectf();
		explicit Rectf(float x, float y, float width, float height);
		explicit Rectf(const glm::vec2& pos, const glm::vec2& size);
		explicit Rectf(const glm::vec2& pos, float width, float height);

		explicit operator Recti() const;

		float x;
		float y;
		float width;
		float height;
	};

	struct Circlei
	{
		Circlei();
		explicit Circlei(const glm::ivec2& center, float radius);
		explicit Circlei(float centerX, float centerY, float radius);

		glm::ivec2 center;
		float radius;
	};

	struct Circlef
	{
		Circlef();
		explicit Circlef(const glm::vec2& center, float radius);
		explicit Circlef(float centerX, float centerY, float radius);

		glm::vec2 center;
		float radius;
	};

	struct Ellipsei
	{
		Ellipsei();
		explicit Ellipsei(int centerX, int centerY, int radiusX, int radiusY);
		explicit Ellipsei(const glm::ivec2& center, int radiusX, int radiusY);

		glm::ivec2 center;
		int radiusX;
		int radiusY;
	};

	struct Ellipsef
	{
		Ellipsef();
		explicit Ellipsef(float centerX, float centerY, float radiusX, float radiusY);
		explicit Ellipsef(const glm::vec2& center, float radiusX, float radiusY);

		glm::vec2 center;
		float radiusX;
		float radiusY;
	};

	///////// OTHER	/////////
	struct Color4f
	{
		Color4f() = default;
		Color4f(float r, float g, float b, float a = 1.f);
		Color4f(int r, int g, int b, int a = 255);

		float r;
		float g;
		float b;
		float a;

		static const Color4f red;
		static const Color4f orange;
		static const Color4f yellow;
		static const Color4f chartreuse;
		static const Color4f green;
		static const Color4f springGreen;
		static const Color4f cyan;
		static const Color4f azure;
		static const Color4f blue;
		static const Color4f violet;
		static const Color4f magenta;
		static const Color4f rose;
		static const Color4f white;
		static const Color4f black;
	};
}

#endif // !STRUCTS_H
