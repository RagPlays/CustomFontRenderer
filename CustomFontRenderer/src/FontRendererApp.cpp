#include "FontRendererApp.h"

#include "Layers/MainLayer.h"
#include "Layers/TestLayer2D.h"

Engine::Application* Engine::CreateApplication(const ApplicationCommandLineArgs& args)
{
	return new FontRendererApp{};
}

FontRendererApp::FontRendererApp()
{
	/*MainLayer* mainLayer{ new MainLayer{} };
	AddLayer(mainLayer);*/
	
	TestLayer2D* testLayer2D{ new TestLayer2D{} };
	AddLayer(testLayer2D);
}