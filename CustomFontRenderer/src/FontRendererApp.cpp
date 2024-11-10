#include "FontRendererApp.h"

using namespace Engine;

Application* Engine::CreateApplication(const ApplicationCommandLineArgs& args)
{
	return new FontRendererApp{};
}

FontRendererApp::FontRendererApp()
{
	MainLayer* mainLayer{ new MainLayer{} };
	AddLayer(mainLayer);
}