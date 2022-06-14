#pragma once

#include "Moon/Core/Platform/PlatformAPI.h"


namespace Moon {

	class Platform
	{
	public:
		static void Init();

		/*
		 * @return Time since initialization in seconds.
		 */
		inline static float GetTime()
		{
			return s_PlatformAPI->GetTime();
		}

	private:
		static Scope<PlatformAPI> s_PlatformAPI;

	};

}
