#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "Engine/Renderer/Camera.h"

namespace Engine
{
	class OrthographicCamera final : public Camera
	{
	public:

		explicit OrthographicCamera(float nearDist = -1.f, float farDist = 1.f);
		explicit OrthographicCamera(float left, float right, float bottom, float top, float nearDist, float farDist);
		virtual ~OrthographicCamera() = default;

		void SetProjection(float left, float right, float bottom, float top);

		virtual void UpdateProjectionMatrix() override;

	private:

		float m_Left;
		float m_Right;
		float m_Bottom;
		float m_Top;

	};
}

#endif // !ORTHOGRAPHICCAMERA_H