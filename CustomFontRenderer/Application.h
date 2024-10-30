#ifndef APPLICATION_H
#define APPLICATION_H

#include "FontParser.h"
#include "FontRenderer.h"
#include "Shader.h"

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

	void UpdateCamera();

private:

	FontParser m_FontParser;
	FontRenderer m_FontRenderer;
	Shader m_Shader;

};

#endif // !APPLICATION_H