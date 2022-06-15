#pragma once

#include "Moon/Core/Util/Platform/PlatformUtilAPI.h"


namespace Moon {

	class Util
	{
	public:
		static void Init();

		/*
		 * @return Time since initialization in seconds.
		 */
		inline static float GetTime()
		{
			return s_PlatformUtilAPI->GetTime();
		}

	private:
		static Scope<PlatformUtilAPI> s_PlatformUtilAPI;

	};

}
