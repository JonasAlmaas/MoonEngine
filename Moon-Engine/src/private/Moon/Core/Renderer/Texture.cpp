#include "mepch.h"
#include "Moon/Core/Renderer/Texture.h"

#include "Moon/Core/Renderer.h"
#include "Moon/Platform/OpenGL/OpenGLTexture.h"


namespace Moon {

	Ref<Texture2D> Texture2D::Create(ImageFormat format, uint32_t width, uint32_t height, const void* data, const TextureProperties& properties)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:			return CreateRef<OpenGLTexture2D>(format, width, height, data, properties);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filepath, const TextureProperties& properties)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:			ME_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:			return CreateRef<OpenGLTexture2D>(filepath, properties);
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
