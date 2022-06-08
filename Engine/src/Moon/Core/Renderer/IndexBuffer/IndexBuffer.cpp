#include "mepch.h"
#include "Moon/Core/Renderer/IndexBuffer/IndexBuffer.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/OpenGL/IndexBuffer/OpenGLIndexBuffer.h"


namespace Moon {

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:			ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::OpenGL:		return new OpenGLIndexBuffer(indices, count);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
