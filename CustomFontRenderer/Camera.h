#ifndef CAMERA_H
#define CAMERA_H

#include "Singleton.h"

#include "Structs.h"

class Camera final : public Singleton<Camera>
{
public:

	~Camera() = default;

	Camera(const Camera& other) = delete;
	Camera(Camera&& other) noexcept = delete;
	Camera& operator=(const Camera& other) = delete;
	Camera& operator=(Camera&& other) noexcept = delete;

	void Move(float x, float y, float z);
	void SetPosition(float x, float y, float z);

	const Matrix4f& GetViewMatrix() const;
	const Matrix4f& GetProjectionMatrix() const;

	const Point3f& GetPosition() const;
	float GetCameraSpeed() const;

	void SetZoom(float zoom);
	float GetZoom() const;
	float GetZoomSpeed() const;

private:

	friend class Singleton<Camera>;
	Camera();

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:

	Point3f m_CameraPos;
	float m_CameraSpeed;
	float m_ZoomSpeed;

	float m_Near;
	float m_Far;
	float m_Zoom;

	Matrix4f m_ViewMatrix;
	Matrix4f m_ProjectionMatrix;

};

#endif // !CAMERA_H