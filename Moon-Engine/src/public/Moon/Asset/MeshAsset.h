#pragma once

#include "Moon/Asset.h"


namespace Moon {

	class MeshAsset : public Asset
	{
	public:
		virtual ~MeshAsset() {}

		virtual AssetType GetAssetType() { return AssetType::Mesh; }

	};

}
