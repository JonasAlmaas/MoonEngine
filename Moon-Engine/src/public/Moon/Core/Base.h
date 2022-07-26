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
#define BIT_MASK(n) (((unsigned long long)0x1) << (n))
#define BYTE_MASK(n) (((unsigned long long)0xff) << ((n) * 8))
#define BIT_MASK_RANGE(start, end) ((BIT_MASK((end) - (start) + 1) - 1) << (start))
#define BYTE_MASK_RANGE(start, end) (BIT_MASK_RANGE((start) * 8, (end + 1) * 8 - 1))

#define BIT_VALUE(x, n) (((x) & BIT_MASK(n)) >> (n))
#define BYTE_VALUE(x, n) ((uint8_t)(((x) & BYTE_MASK(n)) >> ((n) * 8)))
#define BIT_VALUE_RANGE(x, start, end) (((x) & BIT_MASK_RANGE(start, end)) >> (start))
#define BYTE_VALUE_RANGE(x, start, end) (((x) & BYTE_MASK_RANGE(start, end)) >> (start * 8))

#define ME_EXPAND_MACRO(x) x
#define ME_STRINGIFY_MACRO(x) #x

#define ME_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include "Moon/Core/Log.h"
#include "Moon/Core/Assert.h"
#include "Moon/Core/Types.h"
