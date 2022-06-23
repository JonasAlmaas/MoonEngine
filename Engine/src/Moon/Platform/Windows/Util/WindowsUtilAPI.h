#pragma once

#include "Moon/Core/Util/Platform/PlatformUtilAPI.h"


namespace Moon {

	class WindowsUtilAPI : public PlatformUtilAPI
	{
	public:
		WindowsUtilAPI();
		virtual ~WindowsUtilAPI();

		virtual float GetTime() override;

	};

}
