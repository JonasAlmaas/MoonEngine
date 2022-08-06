#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Renderer/Texture.h"


namespace Moon {

	class Font
	{
	public:
		Font(const std::filesystem::path& filepath);
		~Font() = default;

		Ref<Texture2D> GetFontAtlas() const { return m_TextureAtlas; }

		//virtual RendererID GetRendererID() const;
		//virtual void Bind() const;
		virtual bool IsLoaded() const { return m_Loaded; };

	private:
		std::filesystem::path m_Filepath;
		Ref<Texture2D> m_TextureAtlas;

		bool m_Loaded = false;

	};

}
