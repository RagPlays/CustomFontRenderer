#include "Structs.h"

#include <SDL_rect.h>

// LINEI / LINEF //

Linei::Linei()
	: Linei{ 0, 0, 0, 0 }
{
}

Linei::Linei(int x1, int y1, int x2, int y2)
	: pointOne{ x1, y1 }
	, pointTwo{ x2, y2 }
{
}

Linei::Linei(const glm::ivec2& pointOne, const glm::ivec2& pointTwo)
	: pointOne{ pointOne }
	, pointTwo{ pointTwo }
{
}

Linei::operator Linef() const
{
	return Linef
	{
		glm::vec2(pointOne),
		glm::vec2(pointTwo),
	};
}

//

Linef::Linef()
	: Linef{ 0.f, 0.f, 0.f, 0.f }
{
}

Linef::Linef(float x1, float y1, float x2, float y2)
	: pointOne{ x1, y1 }
	, pointTwo{ x2, y2 }
{
}

Linef::Linef(const glm::vec2& pointOne, const glm::vec2& pointTwo)
	: pointOne{ pointOne }
	, pointTwo{ pointTwo }
{
}

Linef::operator Linei() const
{
	return Linei
	{
		glm::ivec2(pointOne),
		glm::ivec2(pointTwo),
	};
}

// RECTI / RECTF //

Recti::Recti()
	: Recti{ 0, 0, 0, 0 }
{
}

Recti::Recti(int x, int y, int width, int height)
	: x{ x }
	, y{ y }
	, width{ width }
	, height{ height }
{
}

Recti::Recti(const glm::ivec2& pos, const glm::ivec2& size)
	: x{ pos.x }
	, y{ pos.y }
	, width{ size.x }
	, height{ size.y }
{
}

Recti::Recti(const glm::ivec2& pos, int width, int height)
	: x{ pos.x }
	, y{ pos.y }
	, width{ width }
	, height{ height }
{
}

Recti::operator Rectf() const
{
	return Rectf
	{
		static_cast<float>(x),
		static_cast<float>(y),
		static_cast<float>(width),
		static_cast<float>(height)
	};
}

Recti::operator SDL_Rect() const
{
	return SDL_Rect
	{
		x,
		y,
		width,
		height
	};
}

Recti::operator SDL_FRect() const
{
	return SDL_FRect
	{
		static_cast<float>(x),
		static_cast<float>(y),
		static_cast<float>(width),
		static_cast<float>(height)
	};
}

//

Rectf::Rectf()
	: Rectf{ 0.f, 0.f, 0.f, 0.f }
{
}

Rectf::Rectf(float x, float y, float width, float height)
	: x{ x }
	, y{ y }
	, width{ width }
	, height{ height }
{
}

Rectf::Rectf(const glm::vec2& pos, const glm::vec2& size)
	: x{ pos.x }
	, y{ pos.y }
	, width{ size.x }
	, height{ size.y }
{
}

Rectf::Rectf(const glm::vec2& pos, float width, float height)
	: x{ pos.x }
	, y{ pos.y }
	, width{ width }
	, height{ height }
{
}

Rectf::operator Recti() const
{
	return Recti
	{
		static_cast<int>(x),
		static_cast<int>(y),
		static_cast<int>(width),
		static_cast<int>(height)
	};
}

Rectf::operator SDL_Rect() const
{
	return SDL_Rect
	{
		static_cast<int>(x),
		static_cast<int>(y),
		static_cast<int>(width),
		static_cast<int>(height)
	};
}

Rectf::operator SDL_FRect() const
{
	return SDL_FRect
	{
		x,
		y,
		width,
		height
	};
}

// CIRCLEI / CIRCLEF //

Circlei::Circlei()
	: Circlei{ 0, 0, 0 }
{
}

Circlei::Circlei(const Point2i& center, float radius)
	: center{ center }
	, radius{ radius }
{
}

Circlei::Circlei(float centerX, float centerY, float radius)
	: center{ centerX, centerY }
	, radius{ radius }
{
}

//

Circlef::Circlef()
	: Circlef{ 0.f, 0.f, 0.f }
{
}

Circlef::Circlef(const Point2f& center, float radius)
	: center{ center }
	, radius{ radius }
{
}

Circlef::Circlef(float centerX, float centerY, float radius)
	: center{ centerX, centerY }
	, radius{ radius }
{
}

// ELLIPSEI / ELLIPSEF //

Ellipsei::Ellipsei()
	: Ellipsei{ 0, 0, 0, 0 }
{
}

Ellipsei::Ellipsei(int centerX, int centerY, int radiusX, int radiusY)
	: center{ centerX, centerY }
	, radiusX{ radiusX }
	, radiusY{ radiusY }
{
}

Ellipsei::Ellipsei(const Point2i& center, int radiusX, int radiusY)
	: center{ center }
	, radiusX{ radiusX }
	, radiusY{ radiusY }
{
}

Ellipsef::Ellipsef()
	: Ellipsef{ 0.f, 0.f, 0.f, 0.f }
{
}

Ellipsef::Ellipsef(float centerX, float centerY, float radiusX, float radiusY)
	: center{ centerX, centerY }
	, radiusX{ radiusX }
	, radiusY{ radiusY }
{
}

Ellipsef::Ellipsef(const Point2f& center, float radiusX, float radiusY)
	: center{ center }
	, radiusX{ radiusX }
	, radiusY{ radiusY }
{
}



// COLOR //

const Color4f Color4f::white	{ 1.0f, 1.0f, 1.0f };
const Color4f Color4f::red		{ 1.0f, 0.0f, 0.0f };
const Color4f Color4f::black	{ 0.0f, 0.0f, 0.0f };
const Color4f Color4f::green	{ 0.0f, 1.0f, 0.0f };
const Color4f Color4f::blue		{ 0.0f, 0.0f, 1.0f };

Color4f::Color4f(float r, float g, float b, float a)
	: r{ r }
	, g{ g }
	, b{ b }
	, a{ a }
{
}

Color4f::Color4f(int r, int g, int b, int a)
	: Color4f
	{
		static_cast<float>(r / 255.f),
		static_cast<float>(g / 255.f),
		static_cast<float>(b / 255.f),
		static_cast<float>(a / 255.f)
	}
{
}