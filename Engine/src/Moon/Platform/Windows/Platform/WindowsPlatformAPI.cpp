#include "mepch.h"
#include "Moon/Platform/Windows/Platform/WindowsPlatformAPI.h"

#include <GLFW/glfw3.h>


namespace Moon {

    WindowsPlatformAPI::WindowsPlatformAPI()
    {
    }

    WindowsPlatformAPI::~WindowsPlatformAPI()
    {
    }

    float WindowsPlatformAPI::GetTime()
    {
        ME_PROFILE_FUNCTION();

        return (float)glfwGetTime();
    }

}

