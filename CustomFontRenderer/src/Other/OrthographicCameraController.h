#ifndef ORTHOGRAPHICCAMERACONTROLLER_H
#define ORTHOGRAPHICCAMERACONTROLLER_H

#include "Engine/Renderer/OrthographicCamera.h"

class OrthographicCameraController final
{
public:

	//OrthographicCameraController() = default;
	explicit OrthographicCameraController(float aspectRatio, float nearDist, float farDist);
	~OrthographicCameraController() = default;

private:

	Engine::OrthographicCamera m_Camera;

};

#endif // !ORTHOGRAPHICCAMERACONTROLLER_H