#ifndef SANDBOXMAINLAYER_H
#define SANDBOXMAINLAYER_H

#include <Engine.h>

#include "CameraControllers/OrthoCamController.h"

class SandboxMainLayer final : public Engine::Layer
{
public:

	SandboxMainLayer();
	virtual ~SandboxMainLayer() = default;

	virtual void OnUpdate() override;

	virtual void OnImGuiRender() override;

	virtual void OnEvent(Engine::Event& e) override;

private:

	// Basics
	void Update();
	void Render() const;

private:

	OrthoCamController m_CameraController;

	glm::vec4 m_SquareColor;
	float m_TriangleRotationZ;
	//Engine::Scope<Engine::Model> m_TriangleModel;
	//Engine::Scope<Engine::Model> m_SquareModel;
	Engine::Ref<Engine::Texture2D> m_Texture;
	Engine::Ref<Engine::Texture2D> m_TransparentTexture;

	// Shaders
	Engine::Ref<Engine::Shader> m_BasicShader;
	Engine::Ref<Engine::Shader> m_FlatColorShader;
	Engine::Ref<Engine::Shader> m_TextureShader;

};

#endif // !SANDBOXMAINLAYER_H
