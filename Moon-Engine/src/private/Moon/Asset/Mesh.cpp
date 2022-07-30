#include "mepch.h"
#include "Moon/Asset/Mesh.h"


namespace Moon {

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

	Ref<Mesh> Mesh::Create(const std::filesystem::path& filepath)
	{
		if (filepath.extension() == ".obj")
			return CreateRef<OBJMesh>(filepath);

		ME_CORE_ASSERT(false, "Unknown file extension!");
	}

	OBJMesh::OBJMesh(const std::filesystem::path& filepath)
	{
		ME_PROFILE_FUNCTION();

		m_Filepath = filepath;

		std::ifstream file(filepath);
		if (file)
		{
			std::vector<glm::vec3> vertexPositions;
			std::vector<glm::vec3> vertexNormals;
			std::vector<glm::vec2> textureUVs;

			Vertex* vertexBufferBase = new Vertex[20000];
			Vertex* vertexBufferPtr = vertexBufferBase;

			uint32_t indexCount = 0;

			std::string line;
			while (std::getline(file, line))
			{
				// Vertex Position
				if (HasLineToken(line, "v "))
				{
					glm::vec3 pos;
					sscanf_s(line.c_str(), "v %f %f %f", &pos.x, &pos.z, &pos.y);
					vertexPositions.push_back(pos);
				}

				// Vertex Normal
				if (HasLineToken(line, "vn"))
				{
					glm::vec3 n;
					sscanf_s(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
					vertexNormals.push_back(n);
				}

				// Texture UV
				if (HasLineToken(line, "vt "))
				{
					glm::vec2 uv;
					sscanf_s(line.c_str(), "vt %f %f", &uv.x, &uv.y);
					textureUVs.push_back(uv);
				}

				// Face / triangle
				if (HasLineToken(line, "f "))
				{
					glm::uvec3 v, n, uv;
					sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v.x, &uv.x, &n.x, &v.z, &uv.z, &n.z, &v.y, &uv.y, &n.y);

					vertexBufferPtr->Position = vertexPositions[v.x - 1];
					vertexBufferPtr->Normal = vertexNormals[n.x - 1];
					vertexBufferPtr->UV = textureUVs[uv.x - 1];
					vertexBufferPtr++;

					vertexBufferPtr->Position = vertexPositions[v.y - 1];
					vertexBufferPtr->Normal = vertexNormals[n.y - 1];
					vertexBufferPtr->UV = textureUVs[uv.y - 1];
					vertexBufferPtr++;

					vertexBufferPtr->Position = vertexPositions[v.z - 1];
					vertexBufferPtr->Normal = vertexNormals[n.z - 1];
					vertexBufferPtr->UV = textureUVs[uv.z - 1];
					vertexBufferPtr++;

					indexCount += 3;
				}
			}

			m_VertexArray = VertexArray::Create();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(indexCount * sizeof(Vertex));

			vertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "Position" },
				{ ShaderDataType::Float3, "Normal" },
				{ ShaderDataType::Float2, "UV" },
				});

			m_VertexArray->AddVertexBuffer(vertexBuffer);

			vertexBuffer->SetData(vertexBufferBase, indexCount * sizeof(Vertex));
			delete[] vertexBufferBase;

			uint32_t* indices = new uint32_t[indexCount];
			for (uint32_t i = 0; i < indexCount; i++)
				indices[i] = i;

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, indexCount);
			m_VertexArray->SetIndexBuffer(indexBuffer);
			delete[] indices;
		}
		else
		{
			SetFlag(AssetFlag::Missing);
			ME_CORE_LOG_ERROR("Failed to open file \"{}\"", filepath);
		}
	}

}
