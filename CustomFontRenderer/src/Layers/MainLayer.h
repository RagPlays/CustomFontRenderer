#ifndef MAINLAYER_H
#define MAINLAYER_H

#include <Engine.h>

class MainLayer final : public Engine::Layer
{
public:

	MainLayer();
	virtual ~MainLayer() = default;

	virtual void OnUpdate() override;

	virtual void OnImGuiRender() override;

private:

	// Basics
	void Update();
	void Render();

	void UpdateCameraMovement();

private:

	const float m_CameraSpeed;
	Engine::Scope<Engine::Camera> m_Camera;

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
