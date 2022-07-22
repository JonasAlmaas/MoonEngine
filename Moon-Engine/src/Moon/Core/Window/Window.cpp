#include "mepch.h"
#include "Moon/Core/Window/Window.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/Windows/Window/WindowsWindow.h"


namespace Moon {

	Scope<Window> Window::Create(const WindowSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateScope<WindowsWindow>(spec);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
