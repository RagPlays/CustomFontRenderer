#ifndef STRUCTS_H
#define STRUCTS_H

#include <glm.hpp>
#include <fwd.hpp>

struct SDL_Rect;
struct SDL_FRect;

struct Linef;
struct Linei;
struct Rectf;
struct Recti;
struct Circlef;
struct Circlei;

//////// SHAPES ////////

using Point2i = glm::ivec2;
using Point2f = glm::vec2;
using Point3i = glm::ivec3;
using Point3f = glm::vec3;

using Matrix4f = glm::mat4;

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
	explicit operator SDL_Rect() const;
	explicit operator SDL_FRect() const;

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
	explicit operator SDL_Rect() const;
	explicit operator SDL_FRect() const;

	float x;
	float y;
	float width;
	float height;
};

struct Circlei
{
	Circlei();
	explicit Circlei(const Point2i& center, float radius);
	explicit Circlei(float centerX, float centerY, float radius);

	Point2i center;
	float radius;
};

struct Circlef
{
	Circlef();
	explicit Circlef(const Point2f& center, float radius);
	explicit Circlef(float centerX, float centerY, float radius);

	Point2f center;
	float radius;
};

struct Ellipsei
{
	Ellipsei();
	explicit Ellipsei(int centerX, int centerY, int radiusX, int radiusY);
	explicit Ellipsei(const Point2i& center, int radiusX, int radiusY);

	Point2i center;
	int radiusX;
	int radiusY;
};

struct Ellipsef
{
	Ellipsef();
	explicit Ellipsef(float centerX, float centerY, float radiusX, float radiusY);
	explicit Ellipsef(const Point2f& center, float radiusX, float radiusY);

	Point2f center;
	float radiusX;
	float radiusY;
};

///////// OTHER	/////////
struct Color4f
{
	Color4f() = default;
	explicit Color4f(float r, float g, float b, float a = 1.f);
	explicit Color4f(int r, int g, int b, int a = 255);

	float r;
	float g;
	float b;
	float a;

	static const Color4f black;
	static const Color4f white;
	static const Color4f red;
	static const Color4f green;
	static const Color4f blue;
};

#endif // !STRUCTS_H
