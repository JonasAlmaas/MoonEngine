#include "mepch.h"
#include "Moon/Core/Renderer/Texture.h"

#include "Moon/Core/Renderer.h"
#include "Moon/Platform/OpenGL/OpenGLTexture.h"


namespace Moon {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, const Texture2DSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLTexture2D>(width, height, spec);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filepath, const Texture2DSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLTexture2D>(filepath, spec);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
