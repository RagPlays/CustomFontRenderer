#include "enginepch.h"
#include "Window.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Engine
{
	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
	#ifdef ENGINE_PLATFORM_WINDOWS
			return std::make_unique<WindowsWindow>(props);
	#else
			ENGINE_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
	#endif
	}

}