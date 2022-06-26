#include "mepch.h"

#ifdef ME_PLATFORM_WINDOWS

#include "Moon/Core/Util/Util.h"

#include <GLFW/glfw3.h>


namespace Moon {

	float Util::GetTime()
	{
		ME_PROFILE_FUNCTION();

		return (float)glfwGetTime();
	}

}

#endif
