#ifndef MAINLAYER_H
#define	MAINLAYER_H

#include <memory>

#include <Engine.h>

#include "CameraControllers/OrthoCamController.h"
#include "Renderer/FontRenderer.h"

class Font;

class MainLayer final : public Engine::Layer
{
public:

	MainLayer();
	~MainLayer();

	virtual void OnAttach() override;

	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;

	virtual void OnEvent(Engine::Event& e) override;

private:

	bool OnButtonReleased(Engine::KeyReleasedEvent& e);

	void Update();
	void Render() const;

private:

	OrthoCamController m_CameraController;
	bool m_InputPossible;
	bool m_DisplayImGui;

	FontRenderConfigs m_CurrentFontRenderConfigs;
	std::shared_ptr<Font> m_Font;
	std::unique_ptr<FontRenderer> m_FontRenderer;

};

#endif // !MAINLAYER_H
