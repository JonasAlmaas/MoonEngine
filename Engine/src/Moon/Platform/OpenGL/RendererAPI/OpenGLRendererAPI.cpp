#include "mepch.h"
#include "Moon/Platform/OpenGL/RendererAPI/OpenGLRendererAPI.h"

#include <glad/glad.h>


namespace Moon {

	void OpenGLRendererAPI::Init()
	{
		ME_PROFILE_FUNCTION();

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

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		ME_PROFILE_FUNCTION();

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}
