#include "mepch.h"
#include "Moon/Platform/Windows/Input/WindowsInput.h"

#include "Moon/Core/Base.h"
#include "Moon/Core/Application/Application.h"

#include <GLFW/glfw3.h>


namespace Moon {

	Ref<Input> Input::s_Instance = CreateRef<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		ME_PROFILE_FUNCTION();

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		ME_PROFILE_FUNCTION();

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		ME_PROFILE_FUNCTION();

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y};
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return y;
	}

}
