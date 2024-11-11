#ifndef MAINLAYER_H
#define MAINLAYER_H

#include <Engine.h>

#include "CameraControllers/OrthographicCameraController.h"

class MainLayer final : public Engine::Layer
{
public:

	MainLayer();
	virtual ~MainLayer() = default;

	virtual void OnUpdate() override;

	virtual void OnImGuiRender() override;

	virtual void OnEvent(Engine::Event& e) override;

private:

	// Basics
	void Update();
	void Render() const;

private:

	OrthographicCameraController m_CameraController;

	Engine::Scope<Engine::Model> m_TriangleModel;
	glm::vec4 m_SquareColor;
	Engine::Scope<Engine::Model> m_SquareModel;
	Engine::Ref<Engine::Texture2D> m_Texture;
	Engine::Ref<Engine::Texture2D> m_TransparentTexture;

	// Shaders
	Engine::Ref<Engine::Shader> m_BasicShader;
	Engine::Ref<Engine::Shader> m_FlatColorShader;
	Engine::Ref<Engine::Shader> m_TextureShader;

};

#endif // !MAINLAYER_H
