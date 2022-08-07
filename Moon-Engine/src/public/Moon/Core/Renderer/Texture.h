#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Asset.h"
#include "Moon/Core/Renderer/Image.h"


namespace Moon {

	class Texture : public Asset
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual ImageFormat GetFormat() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::uvec2 GetSize() const = 0;

		virtual RendererID GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual TextureType GetTextureType() const = 0;
		virtual AssetType GetAssetType() const override { return AssetType::Texture; }

	public:
		bool operator==(const Texture& other) const 
		{
			return GetRendererID() == other.GetRendererID();
		}

	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(ImageFormat format, uint32_t width, uint32_t height, const void* data = nullptr, const TextureProperties& properties = TextureProperties());
		static Ref<Texture2D> Create(const std::filesystem::path& filepath, const TextureProperties& properties = TextureProperties());

		virtual bool Loaded() const = 0;

		virtual TextureType GetTextureType() const override { return TextureType::Texture2D; };

		virtual const std::filesystem::path& GetPath() const = 0;

	};

}
