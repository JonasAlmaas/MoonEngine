#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Asset/FontAsset.h"
#include "Moon/Asset/MeshAsset.h"


namespace Moon {

	class AssetImporter
	{
	public:
		template<typename T>
		static Ref<T> ImportFromFile(const std::filesystem::path& filepath)
		{
			return nullptr;
		}

		template<>
		static Ref<FontAsset> ImportFromFile(const std::filesystem::path& filepath)
		{
			return CreateRef<FontAsset>(filepath);
		}

		template<>
		static Ref<MeshAsset> ImportFromFile(const std::filesystem::path& filepath)
		{
			return ImportMesh(filepath);
		}

	private:
		static Ref<MeshAsset> ImportMesh(const std::filesystem::path& filepath);

	};

}
