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

		// Enable alpha blening
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Enable back face culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);

		// Enable smooth lines
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		ME_PROFILE_RENDERER_FUNCTION();

		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const Color& color)
	{
		ME_PROFILE_RENDERER_FUNCTION();

		Color c = color.GetNormalized();
		glClearColor(c.r, c.g, c.b, c.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		ME_PROFILE_RENDERER_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		ME_PROFILE_RENDERER_FUNCTION();

		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

	const unsigned char* OpenGLRendererAPI::GetGPUVendor()
	{
		return glGetString(GL_VENDOR);
	}

	const unsigned char* OpenGLRendererAPI::GetGPUName()
	{
		return glGetString(GL_RENDERER);
	}

	const unsigned char* OpenGLRendererAPI::GetGPUFirmwareVersion()
	{
		return glGetString(GL_VERSION);
	}

	const void OpenGLRendererAPI::SetMultisample(bool state) const
	{
		if (state)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);
	}

}
