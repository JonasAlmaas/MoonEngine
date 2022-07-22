#include "mepch.h"
#include "Moon/Platform/Windows/Window/WindowsWindow.h"

#include <GLFW/glfw3.h>


namespace Moon {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ME_CORE_LOG_ERROR("GLFE Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowSpecification& spec)
		: m_Specification(spec)
	{
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init()
	{
		ME_PROFILE_FUNCTION();

		m_Data.Title = m_Specification.Title;
		m_Data.Width = m_Specification.Width;
		m_Data.Height = m_Specification.Height;

		ME_CORE_LOG_INFO("Creating window \"{0}\" ({1}, {2})", m_Specification.Title, m_Specification.Width, m_Specification.Height);

		// Initialize GLFW
		if (s_GLFWWindowCount == 0)
		{
			ME_PROFILE_SCOPE("glfwInit");

			int success = glfwInit();
			ME_CORE_ASSERT(success, "Could not initialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
		}


		if (!m_Specification.Decorated)
		{
			glfwWindowHint(GLFW_DECORATED, false);
		}

		if (m_Specification.Fullscreen)
		{
			ME_PROFILE_SCOPE("glfwCreateWindow");

			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

			glfwWindowHint(GLFW_DECORATED, false);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			m_Window = glfwCreateWindow(mode->width, mode->height, m_Data.Title.c_str(), primaryMonitor, nullptr);
			++s_GLFWWindowCount;
		}
		else
		{
			ME_PROFILE_SCOPE("glfwCreateWindow");
			m_Window = glfwCreateWindow((int)m_Specification.Width, (int)m_Specification.Height, m_Data.Title.c_str(), nullptr, nullptr);
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

		// Update window size to actual size
		{
			int width, height;
			glfwGetWindowSize(m_Window, &width, &height);
			m_Data.Width = width;
			m_Data.Height = height;
		}
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

	std::pair<float, float> WindowsWindow::GetWindowPos() const
	{
		int x, y;
		glfwGetWindowPos(m_Window, &x, &y);
		return { (float)x, (float)y };
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		ME_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Specification.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Specification.VSync;
	}

	void WindowsWindow::SetResizable(bool resizable) const
	{
		glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
	}

	void WindowsWindow::Maximize()
	{
		glfwMaximizeWindow(m_Window);
	}

	void WindowsWindow::CenterWindow()
	{
		const GLFWvidmode* videmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int x = (videmode->width / 2) - (m_Data.Width / 2);
		int y = (videmode->height / 2) - (m_Data.Height / 2);
		glfwSetWindowPos(m_Window, x, y);
	}

	void WindowsWindow::EnableCursor()
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void WindowsWindow::DisableCursor()
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void WindowsWindow::SetTitle(const std::string& title)
	{
		m_Data.Title = title;
		glfwSetWindowTitle(m_Window, m_Data.Title.c_str());
	}

}
