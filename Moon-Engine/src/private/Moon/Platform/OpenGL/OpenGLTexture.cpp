#include "mepch.h"
#include "Moon/Platform/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>


namespace Moon {
	
	namespace Utils {

		static GLenum OpenGlSamplerWarp(TextureWrap warp)
		{
			switch (warp)
			{
				case TextureWrap::Repeat:			return GL_REPEAT;
				case TextureWrap::MirroredRepeat:	return GL_MIRRORED_REPEAT;
				case TextureWrap::Clamp:			return GL_CLAMP_TO_EDGE;
				case TextureWrap::ClampToEdge:		return GL_CLAMP_TO_BORDER;
				case TextureWrap::ClampToBorder:	return GL_CLAMP_TO_BORDER;
			}

			ME_CORE_ASSERT(false, "Unknown texture wrap!");
			return 0;
		}

		static GLenum OpenGLSamplerFilter(TextureFilter filter)
		{
			switch (filter)
			{
				case TextureFilter::Linear:			return GL_LINEAR;
				case TextureFilter::Nearest:		return GL_NEAREST;
				//case TextureFilter::Cubic:		//Not supported in OpenGL I dont think
			}

			ME_CORE_ASSERT(false, "Unknown texture filter!");
			return 0;
		}

		static GLenum OpenGLInternalImageFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RED8UN:					return GL_R8;
				case ImageFormat::RED8UI:					return GL_R8;
				case ImageFormat::RED16UI:					return GL_R16UI;
				case ImageFormat::RED32UI:					return GL_R32UI;
				case ImageFormat::RED32F:					return GL_R32F;
				case ImageFormat::RG8:						return GL_RG8;
				case ImageFormat::RG16F:					return GL_RG16F;
				case ImageFormat::RG32F:					return GL_RG32F;
				case ImageFormat::RGB8:						return GL_RGB8;
				case ImageFormat::RGBA8:					return GL_RGBA8;
				case ImageFormat::RGBA16F:					return GL_RGBA16F;
				case ImageFormat::RGBA32F:					return GL_RGBA32F;
				case ImageFormat::SRGB:						return GL_SRGB8;
				//case ImageFormat::DEPTH32FSTENCIL8UINT:		
				//case ImageFormat::DEPTH32F:
				//case ImageFormat::DEPTH24STENCIL8:
				//case ImageFormat::Depth:
			}

			ME_CORE_ASSERT(false, "Unknown image foramt!");
			return 0;
		}

		static GLenum OpenGLDataImageFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RED8UN:
				case ImageFormat::RED8UI:
				case ImageFormat::RED16UI:
				case ImageFormat::RED32UI:
				case ImageFormat::RED32F:
					return GL_RED;
				case ImageFormat::RG8:
				case ImageFormat::RG16F:
				case ImageFormat::RG32F:
					return GL_RG;
				case ImageFormat::RGB8:
					return GL_RGB;
				case ImageFormat::RGBA8:
				case ImageFormat::RGBA16F:
				case ImageFormat::RGBA32F:
					return GL_RGBA;
				case ImageFormat::SRGB:
					return GL_SRGB;

				//case ImageFormat::DEPTH32FSTENCIL8UINT:		
				//case ImageFormat::DEPTH32F:
				//case ImageFormat::DEPTH24STENCIL8:
			}

			ME_CORE_ASSERT(false, "Unknown image foramt!");
			return 0;
		}

		static GLenum OpenGLDataType(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RED8UN:
				case ImageFormat::RED8UI:
				case ImageFormat::RGB8:
				case ImageFormat::RGBA8:
				case ImageFormat::RG8:
				case ImageFormat::SRGB:
					return GL_UNSIGNED_BYTE;
				case ImageFormat::RED16UI:
					return GL_UNSIGNED_SHORT;
				case ImageFormat::RED32UI:
					return GL_UNSIGNED_INT;
				case ImageFormat::RG16F:
				case ImageFormat::RGBA16F:
					return GL_HALF_FLOAT;
				case ImageFormat::RED32F:
				case ImageFormat::RG32F:
				case ImageFormat::RGBA32F:
					return GL_FLOAT;

				//case ImageFormat::DEPTH32FSTENCIL8UINT:		
				//case ImageFormat::DEPTH32F:
				//case ImageFormat::DEPTH24STENCIL8:
			}

			ME_CORE_ASSERT(false, "Unknown image foramt!");
			return 0;
		}

	}

	OpenGLTexture2D::OpenGLTexture2D(ImageFormat format, uint32_t width, uint32_t height, const void* data, const TextureProperties& properties)
		: m_Format(format), m_Width(width), m_Height(height)
	{
		ME_PROFILE_FUNCTION();
		ME_CORE_ASSERT(width != 0 && height != 0, "Texture2D can not be smaller than 1x1!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, Utils::OpenGLInternalImageFormat(m_Format), m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::OpenGLSamplerFilter(properties.SamplerFilter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::OpenGLSamplerFilter(properties.SamplerFilter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Utils::OpenGlSamplerWarp(properties.SamplerWrap));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Utils::OpenGlSamplerWarp(properties.SamplerWrap));

		if (data != nullptr)
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, Utils::OpenGLDataImageFormat(m_Format), Utils::OpenGLDataType(m_Format) , data);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& filepath, const TextureProperties& properties)

	{
		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);

		unsigned char* data = nullptr;
		{
			ME_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
		}


		if (data)
		{
			ME_CORE_ASSERT(width != 0 && height != 0, "Texture2D can not be smaller than 1x1!");
			m_Loaded = true;

			m_Width = width;
			m_Height = height;

			switch (channels)
			{
				case 3:		m_Format = ImageFormat::RGB8; break;
				case 4:		m_Format = ImageFormat::RGBA8; break;
				default:
				{
					ME_CORE_ASSERT(false, "Format not supported!");
					break;
				}
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, Utils::OpenGLInternalImageFormat(m_Format), m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::OpenGLSamplerFilter(properties.SamplerFilter));
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::OpenGLSamplerFilter(properties.SamplerFilter));

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Utils::OpenGlSamplerWarp(properties.SamplerWrap));
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Utils::OpenGlSamplerWarp(properties.SamplerWrap));

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, Utils::OpenGLDataImageFormat(m_Format), Utils::OpenGLDataType(m_Format), data);

			stbi_image_free(data);

		}
		else
		{
			ME_CORE_ASSERT(false, "Failed to load image!");
		}

	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ME_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		ME_PROFILE_FUNCTION();
		
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, Utils::OpenGLDataImageFormat(m_Format), Utils::OpenGLDataType(m_Format), data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		ME_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}

}
