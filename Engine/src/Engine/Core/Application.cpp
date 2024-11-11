#include "enginepch.h"
#include "Application.h"

#include "Engine/Core/Log.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Timer.h"

#include "Engine/ImGui/ImGuiLayer.h"

#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	Application* Application::s_Instance{ nullptr };

	Application::Application()
		: m_Window{ Window::Create() }
		, m_Running{ true }
		, m_Minimized{ false }
		, m_ImguiLayer{ new ImGuiLayer{} }
	{
		// Set Application Instance
		ENGINE_CORE_ASSERT(!s_Instance, "Application can not be set twice");
		s_Instance = this;

		// Set WindowEventCallBack
		m_Window->SetEventCallback(ENGINE_BIND_EVENT_FN(Application::OnEvent));

		// Init Timer and Renderer
		Renderer::Init();
		
		// Set Imgui start layer
		AddOverlay(m_ImguiLayer); 
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
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
		m_LayerContainer.AddLayer(layer); // ownership to container
	}

	void Application::AddOverlay(Layer* layer)
	{
		m_LayerContainer.AddOverlay(layer); // ownership to container
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
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
		for (auto& layer : m_LayerContainer)
		{
			layer->OnUpdate();
		}
	}

	void Application::RenderImGui()
	{
		m_ImguiLayer->Begin();
		for (auto& layer : m_LayerContainer)
		{
			layer->OnImGuiRender();
		}
		m_ImguiLayer->End();
	}

	void Application::Run()
	{
		Timer::Get();

		while (m_Running)
		{
			Timer::Get().Update();

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