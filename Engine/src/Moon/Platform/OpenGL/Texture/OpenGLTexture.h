#pragma once

#include "Moon/Core/Renderer/Texture/Texture.h"

#include <glad/glad.h>


namespace Moon {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, const Texture2DSpecification& spec);
		OpenGLTexture2D(const std::filesystem::path& filepath, const Texture2DSpecification& spec);

		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual const RendererID& GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual Texture2DSpecification GetSpecification() override { return m_Specification; };
		virtual void SetSpecification(const Texture2DSpecification& spec) override;

		virtual const std::filesystem::path& GetPath() const override { return m_Path; };

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		RendererID m_RendererID;

		Texture2DSpecification m_Specification;
		std::filesystem::path m_Path;

		bool m_IsLoaded = false;

		uint32_t m_Width, m_Height;
		GLenum m_InternalFormat, m_DataFormat;

	};

}
