#pragma once

#include "Moon/Core/Renderer/Texture/Texture.h"

#include <glad/glad.h>


namespace Moon {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& filepath);

		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void Bind(uint32_t slot) const override;

		virtual bool operator==(const Texture& other) const override { return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; }

	private:
		uint32_t m_RendererID;
		std::string m_Path;
		uint32_t m_Width, m_Height;
		GLenum m_InternalFormat, m_DataFormat;

	};

}
