#include <GL/glew.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_opengl.h>

#include "Application.h"

#include "Timer.h"
#include "DrawUtils.h"
#include "Camera.h"

Application::Application(int width, int height)
	: m_FontParser{ "../Resources/JetBrainsMono-Bold.ttf" }
	, m_FontRenderer{ &m_FontParser, "This is a test sentence!"}
    , m_Shader{ "Shaders/vs.glsl", "Shaders/fs.glsl" }
{
	Camera::Get();

    m_Shader.Use();
}

void Application::Update()
{
    UpdateCamera();
}

void Application::Render() const
{
    m_FontRenderer.Render();
}

void Application::UpdateCamera()
{
    Camera& cam{ Camera::Get() };

    const float deltaSec{ Timer::Get().GetElapsedSec() };
    float actualMovespeed{ cam.GetCameraSpeed() * deltaSec };

    const Uint8* state{ SDL_GetKeyboardState(nullptr) };
    // Movement
    if (state[SDL_SCANCODE_LSHIFT])
    {
        actualMovespeed *= 2.5f;
    }
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
    {
        cam.Move(-actualMovespeed, 0.f, 0.f);
    }
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
    {
        cam.Move(actualMovespeed, 0.f, 0.f);
    }
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
    {
        cam.Move(0.f, actualMovespeed, 0.f);
    }
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
    {
        cam.Move(0.f, -actualMovespeed, 0.f);
    }

    // Zoom
    const float actualZoomSpeed{ cam.GetZoomSpeed() * deltaSec };
    if (state[SDL_SCANCODE_Q])
    {
        cam.SetZoom(cam.GetZoom() - actualZoomSpeed);

    }
    if (state[SDL_SCANCODE_E])
    {
        cam.SetZoom(cam.GetZoom() + actualZoomSpeed);
    }

    m_Shader.SetUniform("view", cam.GetViewMatrix());
    m_Shader.SetUniform("projection", cam.GetProjectionMatrix());
}