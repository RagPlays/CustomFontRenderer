#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "Engine/Renderer/Camera.h"

namespace Engine
{
	class PerspectiveCamera final : public Camera
	{
	public:

		explicit PerspectiveCamera(float fov, float aspect, float nearDist, float farDist);
		virtual ~PerspectiveCamera() = default;

		void SetFov(float newFov);
		void SetAspectRatio(float newAspect);

	private:

		virtual void UpdateProjectionMatrix() override;

	private:

		float m_FOV;
		float m_AspectRatio;

		float m_Near;
		float m_Far;

	};
}

#endif // !PERSPECTIVECAMERA_H
