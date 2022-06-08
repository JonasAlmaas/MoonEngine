#include "mepch.h"
#include "Moon/Core/Renderer/VertexBuffer/VertexBuffer.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/OpenGL/VertexBuffer/OpenGLVertexBuffer.h"


namespace Moon {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:			ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::OpenGL:		return new OpenGLVertexBuffer(vertices, size);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
