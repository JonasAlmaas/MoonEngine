#pragma once

#include "Moon/Core/Config.h"


// ---- Debug ----
#ifdef ME_DEBUG
	#if defined(ME_PLATFORM_WINDOWS)
		#define ME_DEBUGBREAK() __debugbreak()
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
#else
	#define ME_DEBUGBREAK()
#endif

// ---- General ----
#define BIT(x) (1 << x)

#define ME_EXPAND_MACRO(x) x
#define ME_STRINGIFY_MACRO(x) #x

#define ME_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
