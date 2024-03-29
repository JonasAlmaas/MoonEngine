#include "mepch.h"
#include "Moon/Core/Renderer/Buffer/VertexBuffer.h"

#include "Moon/Core/Renderer.h"
#include "Moon/Platform/OpenGL/Buffer/OpenGLVertexBuffer.h"


namespace Moon {
	
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLVertexBuffer>(size);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
