#include "mepch.h"
#include "Moon/Platform/OpenGL/GraphicsContext/OpenGLContext.h"

#include "Moon/Core/Base.h"

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
		glfwMakeContextCurrent(m_WindowHandle);

		// Initialize Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ME_CORE_ASSERT(status, "Failed to initialize Glad!");

		ME_CORE_INFO("OpenGL Info:");
		ME_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		ME_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		ME_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}
