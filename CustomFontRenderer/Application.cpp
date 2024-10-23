#include "Application.h"

#include "DrawUtils.h"
#include <SDL_opengl.h>

Application::Application(int width, int height)
	: m_FontParser{ "../Resources/JetBrainsMono-Bold.ttf" }
	, m_FontRenderer{ &m_FontParser, "This is a test sentence!"}
{
	Camera::Get();
}

void Application::Update()
{
	Camera::Get().Update();
}

void Application::Render() const
{
	// UI
	/////

	// Game
	const Point2f& camPos{ Camera::Get().GetPosition() };
	glPushMatrix();
	{
		glTranslatef(-camPos.x, -camPos.y, 0.f);

		m_FontRenderer.Render();
	}
	glPopMatrix();
	/////
}