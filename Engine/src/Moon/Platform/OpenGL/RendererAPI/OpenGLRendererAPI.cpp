#include "mepch.h"
#include "Moon/Platform/OpenGL/RendererAPI/OpenGLRendererAPI.h"

#include <glad/glad.h>


namespace Moon {

	void OpenGLRendererAPI::SetClearColor(const Color& color)
	{
		Color c = color.GetNormalized();
		glClearColor(c.r, c.g, c.b, c.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}
