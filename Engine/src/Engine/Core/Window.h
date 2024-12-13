#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <string>
#include <functional>

namespace Engine
{
	class Event;

	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;
		bool vsync;
		bool fullScreen;

		WindowProps()
			: title{ "Engine" }
			, width{ 1600 }
			, height{ 900 }
			, vsync{ false }
			, fullScreen{ false }
		{
		}

		explicit WindowProps(const std::string& name)
			: title{ name }
			, width{ 1600 }
			, height{ 900 }
			, vsync{ false }
			, fullScreen{ false }
		{
		}

		explicit WindowProps(const std::string& title, uint32_t width, uint32_t height, bool vsync, bool fullScreen)
			: title{ title }
			, width{ width }
			, height{ height }
			, vsync{ vsync }
			, fullScreen{ fullScreen }
		{
		}
	};

	class Window
	{
	public:

		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps{});

	};

}

#endif // !WINDOW_H
