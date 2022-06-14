#include "mepch.h"
#include "Moon/Core/Platform/PlatformAPI.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/Windows/Platform/WindowsPlatformAPI.h"


namespace Moon {

    Scope<PlatformAPI> PlatformAPI::Create()
    {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateScope<WindowsPlatformAPI>();
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }

}
