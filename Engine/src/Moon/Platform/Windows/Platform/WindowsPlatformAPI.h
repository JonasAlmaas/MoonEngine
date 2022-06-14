#pragma once

#include "Moon/Core/Platform/PlatformAPI.h"


namespace Moon {

	class WindowsPlatformAPI : public PlatformAPI
	{
	public:
		WindowsPlatformAPI();
		virtual ~WindowsPlatformAPI();

		float GetTime();

	};

}