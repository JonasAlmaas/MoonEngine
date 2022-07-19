#include "mepch.h"
#include "Moon/Platform/Windows/Window/WindowsWindow.h"

#include <GLFW/glfw3.h>


namespace Moon {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ME_CORE_LOG_ERROR("GLFE Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		ME_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		ME_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		ME_CORE_LOG_INFO("Creating window \"{0}\" ({1}, {2})", props.Title, props.Width, props.Height);

		// Initialize GLFW
		if (s_GLFWWindowCount == 0)
		{
			ME_PROFILE_SCOPE("glfwInit");

			int success = glfwInit();
			ME_CORE_ASSERT(success, "Could not initialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			ME_PROFILE_SCOPE("glfwCreateWindow");
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);


		// ---- Set GLFW callbacks ----
		// Window Resize Event
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			bool wasMinimized = (data.Width == 0 || data.Height == 0);
			bool isMinimized = (width == 0 || height == 0);
			bool sendMinimizeEvent = (!wasMinimized && isMinimized) || (wasMinimized && !isMinimized);

			data.Width = width;
			data.Height = height;

			WindowResizeEvent e(width, height);		// Create event
			data.EventCallback(e);					// Dispatch event

			if (sendMinimizeEvent)
			{
				WindowMinimizeEvent e(isMinimized);
				data.EventCallback(e);
			}
		});

		// Window Close Event
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent e;
			data.EventCallback(e);
		});

		// Key Typed Event
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent e(keycode);
			data.EventCallback(e);
		});

		// Key Events
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int keycode, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(keycode, 0);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(keycode);
					data.EventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(keycode, true);
					data.EventCallback(e);
					break;
				}
				default:
					break;
			}
		});

		// Mouse Button Events
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent e(button);
				data.EventCallback(e);
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent e(button);
				data.EventCallback(e);
			}
			default:
				break;
			}
		});

		// Mouse Scrolled Event
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent e((float)xOffset, (float)yOffset);
			data.EventCallback(e);
		});

		// Mouse Moved Event
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent e((float)xPos, (float)yPos);
			data.EventCallback(e);
		});
	}

	void WindowsWindow::Shutdown()
	{
		ME_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		s_GLFWWindowCount--;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		ME_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		ME_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::EnableCursor()
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void WindowsWindow::DisableCursor()
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

}
