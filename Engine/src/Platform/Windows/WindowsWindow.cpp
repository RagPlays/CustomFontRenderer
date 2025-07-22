#include "enginepch.h"
#include "WindowsWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/GraphicsContext.h"

namespace Engine
{
    static uint8_t s_GLFWWindowCount{ 0 };

#if defined ENGINE_DEBUG
    static void GLFWErrorCallback(int error, const char* description)
    {
        ENGINE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }
#endif

    WindowsWindow::WindowsWindow(const WindowProps& props)
        : m_Window{}
        , m_Data{}
    {
        ENGINE_PROFILE_FUNCTION();

        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::OnUpdate()
    {
        ENGINE_PROFILE_FUNCTION();
        {
            ENGINE_PROFILE_SCOPE("WindowsWindow::OnUpdate-glfwPollEvents");
            glfwPollEvents();
        }
        m_Context->SwapBuffers();
    }

    uint32_t WindowsWindow::GetWidth() const
    {
        return m_Data.width;
    }

    uint32_t WindowsWindow::GetHeight() const
    {
        return m_Data.height;
    }

    void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
    {
        m_Data.eventCallback = callback;
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled) glfwSwapInterval(1);
        else glfwSwapInterval(0);
        m_Data.vSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.vSync;
    }

    void* WindowsWindow::GetNativeWindow() const
    {
        return m_Window;
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;

        if (s_GLFWWindowCount == 0)
        {
            {
                ENGINE_PROFILE_SCOPE("WindowsWindow::Init-glfwInit");
                const int success{ glfwInit() };
                ENGINE_CORE_ASSERT_MSG(success, "Could not initialize GLFW!");
            }

            glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);

#if defined ENGINE_DEBUG

            glfwSetErrorCallback(GLFWErrorCallback);
            ENGINE_CORE_INFO("Created window {0} ({1}, {2})", props.title, props.width, props.height);

            if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
            {
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
                glfwWindowHint(GLFW_DEPTH_BITS, 24);
            }
#endif
        }

        {
            ENGINE_PROFILE_SCOPE("WindowsWindow::Init-glfwCreateWindow");

            GLFWmonitor* monitor{ glfwGetPrimaryMonitor() };
            ENGINE_CORE_ASSERT_MSG(monitor, "Failed to get primary monitor!");

            const GLFWvidmode* videoMode{ glfwGetVideoMode(monitor) };
            ENGINE_CORE_ASSERT_MSG(videoMode, "Failed to get video mode!");

            if (props.fullScreen && monitor && videoMode)
            {
                m_Data.width = videoMode->width;
                m_Data.height = videoMode->height;
            }

            m_Window = glfwCreateWindow
            (
                props.width,
                props.height,
                m_Data.title.c_str(),
                props.fullScreen ? monitor : nullptr,
                nullptr
            );
            if (!props.fullScreen)
            {
                glfwSetWindowPos(m_Window, videoMode->width / 2 - props.width / 2, videoMode->height / 2 - props.height / 2);
            }
        }
        ++s_GLFWWindowCount;

        // Context
        m_Context = GraphicsContext::Create(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);

        SetVSync(props.vsync);

        InitCallbacks();
    }

    void WindowsWindow::InitCallbacks()
    {
        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };
            data.width = width;
            data.height = height;

            WindowResizeEvent event{ static_cast<unsigned int>(width), static_cast<unsigned int>(height) };
            data.eventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };
            WindowCloseEvent event{};
            data.eventCallback(event);

        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };

            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressedEvent event{ static_cast<unsigned short>(key), 0 };
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event{ static_cast<unsigned short>(key) };
                data.eventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event{ static_cast<unsigned short>(key), true };
                data.eventCallback(event);
                break;
            }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };

            KeyTypedEvent event{ static_cast<unsigned short>(keycode) };
            data.eventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };

            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event{ static_cast<unsigned short>(button) };
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event{ static_cast<unsigned short>(button) };
                data.eventCallback(event);
                break;
            }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };

            MouseScrolledEvent event{ static_cast<float>(xOffset), static_cast<float>(yOffset) };
            data.eventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };

            MouseMovedEvent event{ static_cast<float>(xPos), static_cast<float>(yPos) };
            data.eventCallback(event);
        });
    }

    void WindowsWindow::Shutdown()
    {
        ENGINE_PROFILE_FUNCTION();

        glfwDestroyWindow(m_Window);
        --s_GLFWWindowCount;

        if (s_GLFWWindowCount == 0)
        {
            glfwTerminate();
        }
    }
}