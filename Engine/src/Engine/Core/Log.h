#ifndef LOG_H
#define LOG_H

#if ENGINE_LOGGING

#include <memory>

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include "Engine/Core/Base.h"

#pragma warning(push)
#pragma warning(disable : 4251)
namespace Engine
{
	class Log final
	{
	public:

		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger();
		static Ref<spdlog::logger>& GetClientLogger();

	private:

		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define ENGINE_CORE_TRACE(...)  ::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ENGINE_CORE_INFO(...)   ::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ENGINE_CORE_WARN(...)   ::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ENGINE_CORE_ERROR(...)  ::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ENGINE_CORE_FATAL(...)  ::Engine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define ENGINE_TRACE(...)  ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...)   ::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define ENGINE_WARN(...)   ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...)  ::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
#define ENGINE_FATAL(...)  ::Engine::Log::GetClientLogger()->critical(__VA_ARGS__)

#pragma warning(pop)

#else

// Core log macros
#define ENGINE_CORE_TRACE(...)
#define ENGINE_CORE_INFO(...)
#define ENGINE_CORE_WARN(...)
#define ENGINE_CORE_ERROR(...)
#define ENGINE_CORE_FATAL(...)

// Client log macros
#define ENGINE_TRACE(...)
#define ENGINE_INFO(...)
#define ENGINE_WARN(...)
#define ENGINE_ERROR(...)
#define ENGINE_FATAL(...)

#endif

#endif // !LOG_H