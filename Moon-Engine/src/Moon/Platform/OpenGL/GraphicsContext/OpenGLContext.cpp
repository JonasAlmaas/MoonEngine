#include "mepch.h"
#include "Moon/Platform/OpenGL/GraphicsContext/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Moon {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		ME_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		ME_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);

		// Initialize Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ME_CORE_ASSERT(status, "Failed to initialize Glad!");

		ME_CORE_INFO("OpenGL Info:");
		ME_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		ME_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		ME_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		ME_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Moon Engine requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		ME_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}
