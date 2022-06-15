#include "mepch.h"
#include "Moon/Platform/Windows/Util/WindowsUtilAPI.h"

#include <GLFW/glfw3.h>


namespace Moon {

	WindowsUtilAPI::WindowsUtilAPI()
	{
	}

	WindowsUtilAPI::~WindowsUtilAPI()
	{
	}

	float WindowsUtilAPI::GetTime()
	{
		ME_PROFILE_FUNCTION();

		return (float)glfwGetTime();
	}

}
