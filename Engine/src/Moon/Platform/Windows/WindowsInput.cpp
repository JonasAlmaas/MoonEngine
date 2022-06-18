#include "mepch.h"

#ifdef ME_PLATFORM_WINDOWS

#include "Moon/Core/Input/Input.h"

#include "Moon/Core/Base.h"
#include "Moon/Core/Application/Application.h"

#include <GLFW/glfw3.h>


namespace Moon {

	bool Input::IsKeyPressed(int keycode)
	{
		ME_PROFILE_FUNCTION();

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		ME_PROFILE_FUNCTION();

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		ME_PROFILE_FUNCTION();

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y};
	}

	float Input::GetMouseX()
	{
		auto[x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto[x, y] = GetMousePosition();
		return y;
	}

}

#endif
