#include "mepch.h"
#include "Moon/Core/Platform/FileDialog/FileDialogAPI.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/Windows/Platform/FileDialog/WindowsFileDialogAPI.h"


namespace Moon {

    Scope<FileDialogAPI> FileDialogAPI::Create()
    {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateScope<WindowsFileDialogAPI>();
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }

}