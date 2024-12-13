#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "Engine/Core/Base.h"
#include "Engine/Core/Assert.h"
#include "Engine/Core/Window.h"
#include "Engine/Core/LayerContainer.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLShader.h"

int main(int argc, char** argv);

namespace Engine
{
	class Camera;
	class Shader;
	class Window;
	class ImGuiLayer;

	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

	struct ApplicationCommandLineArgs
	{
		int count{ 0 };
		char** args{ nullptr };

		const char* operator[](int index) const
		{
			ENGINE_CORE_ASSERT_MSG(index < count, "Index not in args");
			return args[index];
		}
	};

	class Application
	{
	public:

		Application();
		explicit Application(const std::string& name);
		explicit Application(const WindowProps& windowProperties);
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void AddLayer(Layer* layer);
		void AddOverlay(Layer* layer);

		static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() const { return *m_Window; }

	private:

		friend int ::main(int argc, char** argv);

		// Events
		bool OnWindowClosed(WindowCloseEvent& closeWindowE);
		bool OnWindowResize(WindowResizeEvent& resizeWindowE);

		// Updates
		void UpdateLayers();

		// Rendering
		void RenderImGui();

	private:

		std::unique_ptr<Window> m_Window;

		bool m_Running;
		bool m_Minimized;

		LayerContainer m_LayerContainer;

#if ENGINE_IMGUI
		ImGuiLayer* m_ImguiLayer;
#endif

	private:

		static Application* s_Instance;

	};

	// To be defined in client
	Application* CreateApplication(const ApplicationCommandLineArgs& args);
}

#endif // !APPLICATION_H
