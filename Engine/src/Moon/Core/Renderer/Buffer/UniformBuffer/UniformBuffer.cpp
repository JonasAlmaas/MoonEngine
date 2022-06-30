#include "mepch.h"
#include "Moon/Core/Renderer/Buffer/UniformBuffer/UniformBuffer.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/OpenGL/Buffer/UniformBuffer/OpenGLUniformBuffer.h"


namespace Moon {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
