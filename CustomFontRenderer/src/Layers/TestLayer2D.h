#ifndef TESTLAYER2D_H
#define TESTLAYER2D_H

#include <Engine.h>

#include "CameraControllers/OrthographicCameraController.h"

class TestLayer2D : public Engine::Layer
{
public:

	TestLayer2D();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;

	virtual void OnEvent(Engine::Event& e) override;

private:

	void Update();
	void Render() const;

private:

	OrthographicCameraController m_CameraController;

	// temp
	Engine::Ref<Engine::Texture2D> m_TestTexture;
	glm::vec4 m_SquareColor;

};

#endif // !TESTLAYER2D_H