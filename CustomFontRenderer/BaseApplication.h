#ifndef BASEAPPLICATION_H
#define BASEAPPLICATION_H

#include <string>
#include <SDL_video.h>

#include "Application.h"

class BaseApplication final
{
public:

	explicit BaseApplication(const std::string& title, int width, int height);
	~BaseApplication();

	BaseApplication(const BaseApplication& other) = delete;
	BaseApplication(BaseApplication&& other) noexcept = delete;
	BaseApplication& operator=(const BaseApplication& other) = delete;
	BaseApplication& operator=(BaseApplication&& other) noexcept = delete;

	void Run();

private:

	void HandleInput();

private:

	SDL_Window* m_pWindow;
	SDL_GLContext m_pGlContext;
	bool m_Quit;

	std::unique_ptr<Application> m_Application;
	
};

#endif // !BASEAPPLICATION_H