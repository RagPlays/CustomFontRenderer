#ifndef APPLICATION_H
#define APPLICATION_H

#include "Camera.h"
#include "FontParser.h"
#include "FontRenderer.h"

class Application final
{
public:

	explicit Application(int width, int height);
	~Application() = default;

	Application(const Application& other) = delete;
	Application(Application&& other) noexcept = delete;
	Application& operator=(const Application& other) = delete;
	Application& operator=(Application&& other) noexcept = delete;

	void Update();
	void Render() const;

private:

	FontParser m_FontParser;
	FontRenderer m_FontRenderer;

};

#endif // !APPLICATION_H