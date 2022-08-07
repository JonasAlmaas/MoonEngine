#pragma once

#include "Moon/Core/Renderer/Texture.h"


namespace Moon {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(ImageFormat format, uint32_t width, uint32_t height, const void* data, const TextureProperties& properties);
		OpenGLTexture2D(const std::filesystem::path& filepath, const TextureProperties& properties);

		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot) const override;
		virtual ImageFormat GetFormat() const override { return m_Format; }

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual glm::uvec2 GetSize() const override { return { m_Width, m_Height }; }

		virtual RendererID GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual bool Loaded() const override { return m_Loaded; }

		virtual const std::filesystem::path& GetPath() const override { return m_Path; };

	private:
		RendererID m_RendererID;
		std::filesystem::path m_Path;
		uint32_t m_Width;
		uint32_t m_Height;

		bool m_Loaded = false;

		ImageFormat m_Format = ImageFormat::None;

	};

}
