#ifndef TESTLAYER2D_H
#define TESTLAYER2D_H

#include <vector>

#include <Engine.h>

#include "CameraControllers/OrthoCamController.h"

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

	OrthoCamController m_CameraController;

	// temp
	Engine::Ref<Engine::Texture2D> m_CheckerBoardTexture;
	Engine::Ref<Engine::Texture2D> m_TestTexOne;
	Engine::Ref<Engine::Texture2D> m_TestTexTwo;
	Engine::Ref<Engine::Texture2D> m_TestTexThree;
	Engine::Ref<Engine::Texture2D> m_TestTexFour;

	float rotationZ{};
	glm::vec4 m_SquareOneColor;
	glm::vec4 m_SquareTwoColor;

	int nrOfFrames;
	float elapsedTime;

};

#endif // !TESTLAYER2D_H