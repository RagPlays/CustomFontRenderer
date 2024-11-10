#ifndef ASSERT_H
#define ASSERT_H

#include <filesystem>

#include "Engine/Core/Log.h"

#ifdef ENGINE_DEBUG
#define ENGINE_ENABLE_ASSERTS
#endif

#ifdef ENGINE_ENABLE_ASSERTS

//	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
//	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
//	#define ENGINE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { HZ##type##ERROR(msg, __VA_ARGS__); HZ_DEBUGBREAK(); } }
//	#define ENGINE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) HZ_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
//	#define ENGINE_INTERNAL_ASSERT_NO_MSG(type, check) HZ_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", HZ_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
//	
//	#define ENGINE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
//	#define ENGINE_INTERNAL_ASSERT_GET_MACRO(...) HZ_EXPAND_MACRO( HZ_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, HZ_INTERNAL_ASSERT_WITH_MSG, HZ_INTERNAL_ASSERT_NO_MSG) )
//
//	// Currently accepts at least the condition and one additional parameter (the message) being optional
//	#define ENGINE_ASSERT(...) HZ_EXPAND_MACRO( HZ_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
//	#define ENGINE_CORE_ASSERT(...) HZ_EXPAND_MACRO( HZ_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
//#else
//	#define ENGINE_ASSERT(...)
//	#define ENGINE_CORE_ASSERT(...)
//	#endif


#define ENGINE_ASSERT(x, ...) if (!(x)) { ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }
#define ENGINE_CORE_ASSERT(x, ...) if(!(x)) { ENGINE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }
#else
#define ENGINE_ASSERT(x, ...)
#define ENGINE_CORE_ASSERT(x, ...)
#endif

#endif // !ASSERT_H