#include "mepch.h"
#include "Moon/Core/Util/Platform/PlatformUtilAPI.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/Windows/Util/WindowsUtilAPI.h"


namespace Moon {

	Scope<PlatformUtilAPI> PlatformUtilAPI::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateScope<WindowsUtilAPI>();
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
