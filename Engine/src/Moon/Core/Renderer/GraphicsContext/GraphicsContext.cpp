#include "mepch.h"
#include "Moon/Core/Renderer/GraphicsContext/GraphicsContext.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/OpenGL/GraphicsContext/OpenGLContext.h"


namespace Moon {

    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }

}
