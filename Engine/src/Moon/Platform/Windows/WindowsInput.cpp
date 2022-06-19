#include "mepch.h"

#ifdef ME_PLATFORM_WINDOWS

#include "Moon/Core/Input/Input.h"

#include "Moon/Core/Base.h"
#include "Moon/Core/Application/Application.h"
#include "Moon/Core/Input/Code/KeyCode.h"

#include <GLFW/glfw3.h>


namespace Moon {

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		ME_PROFILE_FUNCTION();

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(KeyCode button)
	{
		ME_PROFILE_FUNCTION();

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		ME_PROFILE_FUNCTION();

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y};
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}

}

#endif
