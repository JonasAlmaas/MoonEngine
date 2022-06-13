#pragma once

#include <filesystem>

#include "Moon/Core/Config.h"
#include "Moon/Core/PlatformDetection.h"

// ---- Debug ----
#ifdef ME_DEBUG
	#if defined(ME_PLATFORM_WINDOWS)
		#define ME_DEBUGBREAK() __debugbreak()
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
#else
	#define ME_DEBUGBREAK()
	#define ME_ENABLE_ASSERTS 0
#endif

// ---- General ----
#define BIT(x) (1 << x)

#define ME_EXPAND_MACRO(x) x
#define ME_STRINGIFY_MACRO(x) #x

#define ME_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Moon {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "Moon/Core/Assert.h"
