#pragma once

#include "Moon/Core/Renderer/Texture.h"


namespace Moon {

	struct MSDFData;

	class FontAsset : public Asset
	{
	public:
		FontAsset(const std::filesystem::path& filepath);
		virtual ~FontAsset();

		Ref<Texture2D> GetFontAtlas() const { return m_TextureAtlas; }
		const MSDFData* GetMSDFData() const { return m_MSDFData; }

		virtual AssetType GetAssetType() const override { return AssetType::Font; }

	private:
		std::filesystem::path m_FilePath;
		Ref<Texture2D> m_TextureAtlas;
		MSDFData* m_MSDFData = nullptr;

	};

}
