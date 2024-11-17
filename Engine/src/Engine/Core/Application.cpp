#include "enginepch.h"
#include "Application.h"

#include "Engine/Core/Log.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/FrameTimer.h"

#include "Engine/Renderer/Renderer.h"

#define IMGUI_ENABLED
#if defined IMGUI_ENABLED
#include "Engine/ImGui/ImGuiLayer.h"
#endif

namespace Engine
{
	Application* Application::s_Instance{ nullptr };

	Application::Application()
		: m_Window{ nullptr }
		, m_Running{ true }
		, m_Minimized{ false }
		, m_ImguiLayer{ nullptr }
	{
		ENGINE_PROFILE_FUNCTION();

		// Set Application Instance
		ENGINE_CORE_ASSERT(!s_Instance, "Application can not be set twice");
		s_Instance = this;

		// Create Window
		m_Window = Window::Create();

		// Set WindowEventCallBack
		m_Window->SetEventCallback(ENGINE_BIND_EVENT_FN(Application::OnEvent));

		// Init Timer and Renderer
		Renderer::Init();
		
#if defined IMGUI_ENABLED
		// Set Imgui start layer
		m_ImguiLayer = new ImGuiLayer{};
		AddOverlay(m_ImguiLayer); 
#endif
	}

	Application::~Application()
	{
		ENGINE_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		ENGINE_PROFILE_FUNCTION();

		EventDispatcher dispatcher{ e };

		dispatcher.Dispatch<WindowCloseEvent>(ENGINE_BIND_EVENT_FN(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it{ m_LayerContainer.rbegin() }; it != m_LayerContainer.rend(); ++it)
		{
			if (e.Handled) break;
			(*it)->OnEvent(e);
		}
	}

	void Application::AddLayer(Layer* layer)
	{
		ENGINE_PROFILE_FUNCTION();

		m_LayerContainer.AddLayer(layer); // ownership to container
	}

	void Application::AddOverlay(Layer* layer)
	{
		ENGINE_PROFILE_FUNCTION();

		m_LayerContainer.AddOverlay(layer); // ownership to container
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		ENGINE_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
		}
		else
		{
			m_Minimized = false;
			Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		}

		return false;
	}

	void Application::UpdateLayers()
	{
		ENGINE_PROFILE_FUNCTION();

		for (auto& layer : m_LayerContainer)
		{
			layer->OnUpdate();
		}
	}

	void Application::RenderImGui()
	{
#if defined IMGUI_ENABLED

		ENGINE_PROFILE_FUNCTION();

		m_ImguiLayer->Begin();
		for (auto& layer : m_LayerContainer)
		{
			layer->OnImGuiRender();
		}
		m_ImguiLayer->End();
#endif
	}

	void Application::Run()
	{
		ENGINE_PROFILE_FUNCTION();

		FrameTimer::Get();

		while (m_Running)
		{
			ENGINE_PROFILE_SCOPE("Applicstion::Run-RunLoop");

			FrameTimer::Get().Update();

			if (!m_Minimized)
			{
				UpdateLayers();

				RenderImGui();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}
}