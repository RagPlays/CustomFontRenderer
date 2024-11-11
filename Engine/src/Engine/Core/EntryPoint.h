#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "Engine/Core/Log.h"
#include "Engine/Core/Application.h"

#ifdef ENGINE_PLATFORM_WINDOWS

#include <iostream>

extern Engine::Application* Engine::CreateApplication(const ApplicationCommandLineArgs& args);

int main(int argc, char** argv)
{
	// Initialize logger
	Engine::Log::Init();

#if defined ENGINE_DEBUG
	ENGINE_CORE_INFO("Initilized Log.");
#endif

	// Starting application
	Engine::Application* app{ Engine::CreateApplication({argc, argv}) };

#if defined ENGINE_DEBUG
	ENGINE_CORE_INFO("Applicaton Created.");
#endif

	// Running application
	app->Run();

#if defined ENGINE_DEBUG
	ENGINE_CORE_INFO("Shutting Down Application.");
#endif

	// Shutting down application
	delete app;

#if defined ENGINE_DEBUG
	ENGINE_CORE_INFO("Application Terminated.");

	std::cin.get();
#endif

}

#endif

#endif // !ENTRYPOINT_H