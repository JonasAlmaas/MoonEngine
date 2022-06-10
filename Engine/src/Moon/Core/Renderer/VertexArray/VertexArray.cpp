#include "mepch.h"
#include "Moon/Core/Renderer/VertexArray/VertexArray.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/OpenGL/VertexArray/OpenGLVertexArray.h"


namespace Moon {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return new OpenGLVertexArray();
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
