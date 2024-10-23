#include "BaseApplication.h"

#include <stdexcept>

#include <SDL.h>
#include <SDL_opengl.h>

#include "Timer.h"

#if defined DEBUG || _DEBUG
#include <iostream>
void DLog(const std::string& message)
{
	std::cout << message;
}
#else
void DLog(const std::string& message){}
#endif // DEBUG || _DEBUG

BaseApplication::BaseApplication(const std::string& title, int width, int height)
	: m_pWindow{ nullptr }
	, m_pGlContext{ nullptr }
	, m_Quit{ false }
	, m_Application{ nullptr }
{
	Timer& timer{ Timer::Get() };
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		throw std::runtime_error{ std::string{ "SDL_Init Error: " } + SDL_GetError() };
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_pWindow = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);
	if (!m_pWindow)
	{
		throw std::runtime_error{ std::string{ "SDL_Window Error: " } + SDL_GetError() };
	}

	m_pGlContext = SDL_GL_CreateContext(m_pWindow);
	if (!m_pGlContext)
	{
		throw std::runtime_error{ std::string{ "GlContext Error: " } + SDL_GetError() };
	}

	DLog("OpenGL version: " + std::string{ reinterpret_cast<const char*>(glGetString(GL_VERSION)) } + "\n");
	DLog("GLSL version: " + std::string{ reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)) } + "\n");

	// Set the Projection matrix to the identity matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set up a two-dimensional orthographic viewing region.
	glOrtho(0, width, 0, height, -1, 1); // y from bottom to top

	// Set the viewport to the client window area
	// The viewport is the rectangular region of the window where the image is drawn.
	glViewport(0, 0, width, height);

	// Set the Modelview matrix to the identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Disable depth test due to 2D environment
	glDisable(GL_DEPTH_TEST);

	// Enable color blending and use alpha blending (only needed when drawing with transparency)
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create application
	m_Application = std::make_unique<Application>(width, height);
}

BaseApplication::~BaseApplication()
{
	// Delete openGl context
	SDL_GL_DeleteContext(m_pGlContext);
	m_pGlContext = nullptr;

	// Delete SDL Window
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;

	//Mix_Quit();
	//TTF_Quit();
	SDL_Quit();
}

void BaseApplication::Run()
{
	Timer& timer{ Timer::Get() };

	while (!m_Quit)
	{
		HandleInput();

		timer.Update();

		m_Application->Update();

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Application->Render();

		SDL_GL_SwapWindow(m_pWindow);
	}
}

void BaseApplication::HandleInput()
{
	SDL_Event e{};
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			m_Quit = true;
			return;
		}
	}
}