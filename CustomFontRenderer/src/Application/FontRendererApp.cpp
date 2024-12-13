#include "FontRendererApp.h"

#include "Layers/MainLayer.h"

Engine::Application* Engine::CreateApplication(const ApplicationCommandLineArgs& args)
{
	return new FontRendererApp{};
}

FontRendererApp::FontRendererApp()
	: Application{ Engine::WindowProps{ "Custom Font Renderer", 1920, 1080, true, false} }
{
	MainLayer* mainLayer{ new MainLayer{} };
	AddLayer(mainLayer);
}

FontRendererApp::~FontRendererApp()
{
}