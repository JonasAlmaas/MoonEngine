#include "mepch.h"
#include "Moon/Core/Renderer/Buffer/IndexBuffer.h"

#include "Moon/Core/Renderer.h"
#include "Moon/Platform/OpenGL/Buffer/OpenGLIndexBuffer.h"


namespace Moon {

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
