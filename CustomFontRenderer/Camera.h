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

	void Update();

	const Point2f& GetPosition() const;

private:

	friend class Singleton<Camera>;
	Camera();

private:

	Point2f m_CameraPos;
	float m_CameraSpeed;

};

#endif // !CAMERA_H