#pragma once

#include "Moon/Core/Assert.h"


#ifdef ME_DEBUG
	#if defined(ME_PLATFORM_WINDOWS)
		#define ME_DEBUGBREAK() __debugbreak()
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define ME_ENABLE_ASSERTS
#else
	#define ME_DEBUGBREAK()
#endif

#define ME_EXPAND_MACRO(x) x
#define ME_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define ME_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)


namespace Moon {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}
