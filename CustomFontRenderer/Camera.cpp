#include "Camera.h"

#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>

#include "Timer.h"

Camera::Camera() // in private
	: m_CameraPos{}
    , m_CameraSpeed{ 1000.f }
{
}

void Camera::Update()
{
    const float deltaSec{ Timer::Get().GetElapsedSec() };

    float actualMovespeed{ m_CameraSpeed * deltaSec };

	const Uint8* state{ SDL_GetKeyboardState(nullptr) };
    if (state[SDL_SCANCODE_LSHIFT])
    {
        actualMovespeed *= 2.5f;
    }
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
    {
        m_CameraPos.x -= actualMovespeed;
    }
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
    {
        m_CameraPos.x += actualMovespeed;
    }
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
    {
        m_CameraPos.y += actualMovespeed;
    }
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
    {
        m_CameraPos.y -= actualMovespeed;
    }
}

const Point2f& Camera::GetPosition() const
{
    return m_CameraPos;
}