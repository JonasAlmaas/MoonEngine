#include "mepch.h"
#include "Moon/Core/Renderer/Texture/Texture.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Platform/OpenGL/Texture/OpenGLTexture.h"


namespace Moon {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLTexture2D>(width, height);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLTexture2D>(filepath);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
