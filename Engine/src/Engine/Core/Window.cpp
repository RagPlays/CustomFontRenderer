#include "enginepch.h"
#include "Window.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Engine
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef ENGINE_PLATFORM_WINDOWS
			return CreateScope<WindowsWindow>(props);
	#else
			ENGINE_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
	#endif
	}
}