#include "mepch.h"
#include "Moon/Platform/OpenGL/Texture/OpenGLTexture.h"

#include <stb_image.h>


namespace Moon {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		ME_PROFILE_FUNCTION();

		ME_CORE_ASSERT(width != 0 && height != 0, "Texture2D can not be smaller than 1x1!");

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
		: m_Path(filepath)
	{
		ME_PROFILE_FUNCTION();

		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);

		unsigned char* data = nullptr;
		{
			ME_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		}

		ME_CORE_ASSERT(data, "Failed to load image!");
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

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
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

	void OpenGLTexture2D::Unbind() const
	{
		ME_PROFILE_FUNCTION();

	}

}
