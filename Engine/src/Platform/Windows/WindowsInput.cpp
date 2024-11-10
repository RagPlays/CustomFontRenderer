#include "enginepch.h"
#include "WindowsInput.h"

#include <GLFW/glfw3.h>

#include "Engine/Core/Application.h"

namespace Engine
{
    // For now init it here, you can set it in init of windowsWindow as well to define multiple input platforms
    Input* Input::s_Instance{ new WindowsInput{} };

    bool WindowsInput::IsKeyPressedImpl(KeyCode key)
    {
        const Application& app{ Application::Get() };
        GLFWwindow* window{ static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow()) };
        int state{ glfwGetKey(window, key) };

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(MouseCode button)
    {
        const Application& app{ Application::Get() };
        GLFWwindow* window{ static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow()) };
        int state{ glfwGetMouseButton(window, button) };

        return state == GLFW_PRESS;
    }

    glm::vec2 WindowsInput::GetMousePositionImpl()
    {
        const Application& app{ Application::Get() };
        GLFWwindow* window{ static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow()) };
        double xpos{};
        double ypos{};
        glfwGetCursorPos(window, &xpos, &ypos);

        return glm::vec2{ xpos, ypos };
    }

    float WindowsInput::GetMouseXImpl()
    {
        return GetMousePositionImpl().x;
    }

    float WindowsInput::GetMouseYImpl()
    {
        return GetMousePositionImpl().y;
    }
}