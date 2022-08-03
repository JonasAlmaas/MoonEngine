#pragma once

#include "Moon/Asset.h"
#include "Moon/Core/Renderer/VertexArray.h"


namespace Moon {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;
	};

	class MeshAsset : public Asset
	{
	public:
		MeshAsset() = default;
		virtual ~MeshAsset() = default;

		Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }

		virtual AssetType GetAssetType() override { return AssetType::Mesh; }

	protected:
		std::filesystem::path m_Filepath;
		Ref<VertexArray> m_VertexArray;

	private:
		friend Ref<MeshAsset> MeshFromObj(const std::filesystem::path&);

	};

}
