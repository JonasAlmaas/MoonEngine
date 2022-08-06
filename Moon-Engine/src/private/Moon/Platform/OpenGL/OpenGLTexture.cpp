#include "mepch.h"
#include "Moon/Platform/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>


namespace Moon {

	namespace Utils {

		static GLint Texture2DFilterToGlint(const Texture2DFilter& filter)
		{
			switch (filter)
			{
				case Texture2DFilter::Linear:					return GL_LINEAR;
				case Texture2DFilter::Nearest:					return GL_NEAREST;
				case Texture2DFilter::LinearMipmapLinear:		return GL_LINEAR_MIPMAP_LINEAR;
				case Texture2DFilter::LinearMipmapNearest:		return GL_LINEAR_MIPMAP_NEAREST;
				case Texture2DFilter::NearestMipmapLinear:		return GL_NEAREST_MIPMAP_LINEAR;
				case Texture2DFilter::NearestMipmapNearest:		return GL_NEAREST_MIPMAP_NEAREST;
			}

			ME_CORE_ASSERT(false, "Unknown Texture 2D Filter!");
			return GL_LINEAR;
		}

		static GLint Texture2DWrapToGlint(const Texture2DWrap& wrap)
		{
			switch (wrap)
			{
				case Texture2DWrap::Repeat:				return GL_REPEAT;
				case Texture2DWrap::MirroredRepeat:		return GL_MIRRORED_REPEAT;
				case Texture2DWrap::ClampToEdge:		return GL_CLAMP_TO_EDGE;
				case Texture2DWrap::ClampToBorder:		return GL_CLAMP_TO_BORDER;
			}

			ME_CORE_ASSERT(false, "Unknown Texture 2D Wrap!");
			return GL_REPEAT;
		}

	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, const Texture2DSpecification& spec)
		: m_Width(width), m_Height(height), m_Specification(spec)
	{
		ME_PROFILE_FUNCTION();

		ME_CORE_ASSERT(width != 0 && height != 0, "Texture2D can not be smaller than 1x1!");

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::Texture2DFilterToGlint(spec.MinFilter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::Texture2DFilterToGlint(spec.MagFilter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Utils::Texture2DWrapToGlint(spec.WrapS));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Utils::Texture2DWrapToGlint(spec.WrapT));
	}
	
	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& filepath, const Texture2DSpecification& spec)
		: m_Path(filepath), m_Specification(spec)
	{
		ME_PROFILE_FUNCTION();

		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);

		unsigned char* data = nullptr;
		{
			ME_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			m_IsLoaded = true;

			ME_CORE_ASSERT(width != 0 && height != 0, "Texture2D can not be smaller than 1x1!");

			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			switch (channels)
			{
				case 3:
				{
					internalFormat = GL_RGB8;
					dataFormat = GL_RGB;
					break;
				}
				case 4:
				{
					internalFormat = GL_RGBA8;
					dataFormat = GL_RGBA;
					break;
				}
				default:
				{
					ME_CORE_ASSERT(false, "Format not supported!");
					break;
				}
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::Texture2DFilterToGlint(spec.MinFilter));
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::Texture2DFilterToGlint(spec.MagFilter));

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Utils::Texture2DWrapToGlint(spec.WrapS));
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Utils::Texture2DWrapToGlint(spec.WrapT));

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			ME_CORE_ASSERT(data, "Failed to load image!");
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

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		ME_CORE_ASSERT(size == (m_Width * m_Height * bpp), "Data must be the entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		ME_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::SetSpecification(const Texture2DSpecification& spec)
	{
		m_Specification = spec;

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::Texture2DFilterToGlint(spec.MinFilter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::Texture2DFilterToGlint(spec.MagFilter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Utils::Texture2DWrapToGlint(spec.WrapS));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Utils::Texture2DWrapToGlint(spec.WrapT));
	}

}
