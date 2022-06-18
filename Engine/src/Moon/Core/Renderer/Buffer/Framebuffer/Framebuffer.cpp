#include "mepch.h"
#include "Moon/Core/Renderer/Buffer/Framebuffer/Framebuffer.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/OpenGL/Buffer/FrameBuffer/OpenGLFramebuffer.h"


namespace Moon {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLFramebuffer>(spec);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
