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

	class Mesh : public Asset
	{
	public:
		virtual ~Mesh() = default;

		Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }

		virtual AssetType GetAssetType() { return AssetType::Mesh; }

		static Ref<Mesh> Create(const std::filesystem::path& filepath);

	protected:
		std::filesystem::path m_Filepath;
		Ref<VertexArray> m_VertexArray;

	};

	class OBJMesh : public Mesh
	{
	public:
		OBJMesh(const std::filesystem::path& filepath);
		virtual ~OBJMesh() {}

	};

}
