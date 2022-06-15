#include "mepch.h"
#include "Moon/Platform/OpenGL/RendererAPI/OpenGLRendererAPI.h"

#include <glad/glad.h>


namespace Moon {

	void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:			ME_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:			ME_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:				ME_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION:	ME_CORE_TRACE(message); return;
		}

		ME_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init()
	{
		ME_PROFILE_FUNCTION();
		
		#ifdef ME_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		ME_PROFILE_FUNCTION();

		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const Color& color)
	{
		ME_PROFILE_FUNCTION();

		Color c = color.GetNormalized();
		glClearColor(c.r, c.g, c.b, c.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		ME_PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		ME_PROFILE_FUNCTION();

		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

}
