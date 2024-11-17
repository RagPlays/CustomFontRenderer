#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "Engine/Core/Base.h"
#include "Engine/Core/Application.h"

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication(const ApplicationCommandLineArgs& args);

int main(int argc, char** argv)
{
#if ENGINE_LOGGING
	Engine::Log::Init();
#endif

	ENGINE_PROFILE_BEGIN_SESSION("Startup", "EngineProfile-Startup.json");
	Engine::Application* app{ Engine::CreateApplication({argc, argv}) };
	ENGINE_PROFILE_END_SESSION();

	ENGINE_PROFILE_BEGIN_SESSION("Runtime", "EngineProfile-Runtime.json");
	app->Run();
	ENGINE_PROFILE_END_SESSION();

	ENGINE_PROFILE_BEGIN_SESSION("Shutdown", "EngineProfile-Shutdown.json");
	delete app;
	ENGINE_PROFILE_END_SESSION();
}

#endif

#endif // !ENTRYPOINT_H