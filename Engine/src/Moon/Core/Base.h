#pragma once


#ifdef ME_PLATFORM_WINDOWS

#else
	#error MoonEngine only supports Windows!
#endif

#ifdef ME_DEBUG
	#define ME_ENABLE_ASSERTS
#endif

#ifdef ME_ENABLE_ASSERTS
	#define ME_ASSERT(x, ...)		{ if (!(x))	{ ME_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak; }}
	#define ME_CORE_ASSERT(x, ...)	{ if (!(x))	{ ME_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak; }}
#else
	#define ME_ASSERT(x, ...)
	#define ME_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define ME_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
