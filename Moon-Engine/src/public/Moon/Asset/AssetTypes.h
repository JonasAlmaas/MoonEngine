#pragma once

#include "Moon/Core/Base.h"


namespace Moon {

	enum class AssetFlag : uint16_t
	{
		None = 0,
		Missing = BIT_MASK(0),
		Invalid = BIT_MASK(1),
	};

	enum class AssetType : uint16_t
	{
		None = 0,
		Mesh,
		Font,
		Texture,
	};

	namespace Utils {

		inline AssetType AssetTypeFromString(const std::string& assetType)
		{
			if (assetType == "None")				return AssetType::None;
			if (assetType == "Mesh")				return AssetType::Mesh;
			if (assetType == "Font")				return AssetType::Font;
			if (assetType == "Texture")				return AssetType::Texture;

			ME_CORE_ASSERT(false, "Unknown Asset Type");
			return AssetType::None;
		}

		inline const char* AssetTypeToString(AssetType assetType)
		{
			switch (assetType)
			{
				case AssetType::None:					return "None";
				case AssetType::Mesh:					return "Mesh";
				case AssetType::Font:					return "Font";
				case AssetType::Texture:				return "Texture";
			}

			ME_CORE_ASSERT(false, "Unknown Asset Type");
			return "None";
		}

	}

}
