#pragma once


#ifdef ME_DEBUG
	#define ME_ENABLE_ASSERTS 1
#else
	#define ME_ENABLE_ASSERTS 0
#endif

#define ME_ENABLE_PROFILING 0
#define ME_ENABLE_PROFILING_RENDERER 0

#define ME_ENABLE_RENDERER2D_STATISTICS 1