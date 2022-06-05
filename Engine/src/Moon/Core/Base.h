#pragma once


#ifdef ME_PLATFORM_WINDOWS
	#ifdef ME_BUILD_DLL
		#define MOON_API __declspec(dllexport)
	#else
		#define MOON_API __declspec(dllimport)
	#endif
#else
	#error MoonEngine only supports Windows!
#endif
