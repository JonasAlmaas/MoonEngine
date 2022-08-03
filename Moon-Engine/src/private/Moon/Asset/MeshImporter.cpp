#include "mepch.h"
#include "Moon/Asset/AssetImporter.h"


namespace Moon {

	namespace Utils {

		static bool HasLineToken(const std::string& line, const char* token)
		{
			size_t tokenLength = strlen(token);
			if (line.size() < tokenLength)
				return false;

			for (size_t i = 0; i < tokenLength; i++)
			{
				if (line[i] == token[i])
					continue;
				else
					return false;
			}

			return true;
		}

	}

	static Ref<MeshAsset> MeshFromObj(const std::filesystem::path& filepath)
	{
		ME_PROFILE_FUNCTION();

		Ref<MeshAsset> mesh = CreateRef<MeshAsset>();
		mesh->m_Filepath = filepath;

		std::ifstream file(filepath);
		if (file)
		{
			std::vector<glm::vec3> positions;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> textureUVs;

			std::vector<Vertex> vertices;

			std::string line;
			while (std::getline(file, line))
			{
				// Vertex Position
				if (Utils::HasLineToken(line, "v "))
				{
					glm::vec3 pos;
					sscanf_s(line.c_str(), "v %f %f %f", &pos.x, &pos.z, &pos.y);
					positions.push_back(pos);
				}

				// Vertex Normal
				else if (Utils::HasLineToken(line, "vn"))
				{
					glm::vec3 n;
					sscanf_s(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
					normals.push_back(n);
				}

				// Texture UV
				else if (Utils::HasLineToken(line, "vt "))
				{
					glm::vec2 uv;
					sscanf_s(line.c_str(), "vt %f %f", &uv.x, &uv.y);
					textureUVs.push_back(uv);
				}

				// Face / triangle
				else if (Utils::HasLineToken(line, "f "))
				{
					glm::uvec3 v, n, uv;
					sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v.x, &uv.x, &n.x, &v.z, &uv.z, &n.z, &v.y, &uv.y, &n.y);

					vertices.emplace_back(Vertex(positions[v.x - 1], normals[n.x - 1], textureUVs[uv.x - 1]));
					vertices.emplace_back(Vertex(positions[v.y - 1], normals[n.y - 1], textureUVs[uv.y - 1]));
					vertices.emplace_back(Vertex(positions[v.z - 1], normals[n.z - 1], textureUVs[uv.z - 1]));
				}
			}

			mesh->m_VertexArray = VertexArray::Create();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vertices[0], (uint32_t)vertices.size() * sizeof(Vertex));

			vertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "Position" },
				{ ShaderDataType::Float3, "Normal" },
				{ ShaderDataType::Float2, "UV" },
			});
			mesh->m_VertexArray->AddVertexBuffer(vertexBuffer);

			// Generate indices
			uint16_t indexCount = (uint16_t)vertices.size();

			uint32_t* indices = new uint32_t[indexCount];
			for (uint32_t i = 0; i < indexCount; i++)
				indices[i] = i;

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, indexCount);
			mesh->m_VertexArray->SetIndexBuffer(indexBuffer);

			delete[] indices;

		}
		else
		{
			mesh->SetFlag(AssetFlag::Missing);
			ME_CORE_LOG_ERROR("Failed to open file \"{}\"", filepath);
		}

		return mesh;
	}

	Ref<MeshAsset> AssetImporter::ImportMesh(const std::filesystem::path& filepath)
	{
		auto extention = filepath.extension();

		if (extention == ".obj")
			return MeshFromObj(filepath);

		ME_CORE_LOG_ERROR("Can not import mesh from a {} file!", extention);
		return nullptr;
	}

}
