#include "mepch.h"
#include "Moon/Core/Renderer/VertexArray/VertexArray.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/OpenGL/VertexArray/OpenGLVertexArray.h"


namespace Moon {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLVertexArray>();
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
