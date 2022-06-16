#include "mepch.h"
#include "Moon/Renderer/Renderer2D/Renderer2D.h"

#include "Moon/Core/Renderer/IndexBuffer/IndexBuffer.h"
#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"
#include "Moon/Core/Renderer/VertexArray/VertexArray.h"
#include "Moon/Core/Renderer/VertexBuffer/VertexBuffer.h"
#include "Moon/Core/Renderer/Shader/Shader.h"
#include "Moon/Core/Renderer/Texture/Texture.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Moon {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 UV;
	};

	struct Renderer2DData
	{
		// Max per drawcall
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> Shader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		glm::vec4 QuadVertexPositions[4];
		glm::vec2 QuadUVCoords[4];
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		ME_PROFILE_FUNCTION();

		// Generate a 1x1 white texture
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t textureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&textureData, sizeof(uint32_t));

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxQuads * sizeof(QuadVertex));

		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "Position XYZ" },
			{ ShaderDataType::Float4, "Color" },
			{ ShaderDataType::Float2, "UV" },
		};
		s_Data.QuadVertexBuffer->SetLayout(layout);

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		// Create indices for "s_Data.MaxQuads" quads.
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.Shader = Shader::Create("assets/shaders/2D.glsl");
		s_Data.Shader->Bind();
		s_Data.Shader->SetInt("u_Texture", 0);

		// ---- Create basic quad information -----
		s_Data.QuadVertexPositions[0] = { -0.5, -0.5, 0, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5, -0.5, 0, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5,  0.5, 0, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5,  0.5, 0, 1.0f };

		s_Data.QuadUVCoords[0] = { 0, 0 };
		s_Data.QuadUVCoords[1] = { 1.0f, 0.0f };
		s_Data.QuadUVCoords[2] = { 1.0f, 1.0f };
		s_Data.QuadUVCoords[3] = { 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		ME_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		ME_PROFILE_FUNCTION();

		s_Data.Shader->Bind();
		s_Data.Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		ME_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::Flush()
	{
		ME_PROFILE_FUNCTION();

		// Nothing to draw
		if (s_Data.QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		s_Data.WhiteTexture->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.WhiteTexture->Unbind();
	}

	// ---- Primitives ----
	
	void Renderer2D::Ultra_DrawQuad(const glm::mat4& transform, const Ref<Texture> texture, const glm::vec2& tileFactor, const Color& tint)
	{
		ME_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tint.Format == ColorFormat::RGBANormalized ? tint : tint.GetNormalized();
			s_Data.QuadVertexBufferPtr->UV = tileFactor * s_Data.QuadUVCoords[i];
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::Super_DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		ME_PROFILE_FUNCTION();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size , 0.0f });
		glm::mat4 transform = translation * scale;

		Ultra_DrawQuad(transform, texture, tileFactor, tint);
	}

	void Renderer2D::Super_DrawRotatedQuad(const glm::vec3& position, float rotationRadians, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		ME_PROFILE_FUNCTION();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotationRadians, { 0.0f, 0.0f, 1.0f });
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size , 0.0f });
		glm::mat4 transform = translation * rotation * scale;

		Ultra_DrawQuad(transform, texture, tileFactor, tint);
	}

	// -- Draw Quad --

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Color& color)
	{
		Super_DrawQuad({ position, 0.0f }, { size, size }, s_Data.WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Color& color)
	{
		Super_DrawQuad(position, { size, size }, s_Data.WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, { size, size * aspectRatio }, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, { size, size * aspectRatio }, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad({ position, 0.0f }, { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawQuad(position, { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
		Super_DrawQuad({ position, 0.0f }, size, s_Data.WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Color& color)
	{
		Super_DrawQuad(position, size, s_Data.WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawQuad({ position, 0.0f }, size, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawQuad(position, size, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawQuad({ position, 0.0f }, size, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawQuad(position, size, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawQuad({ position, 0.0f }, size, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawQuad(position, size, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawQuad({ position, 0.0f }, size, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawQuad(position, size, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawQuad({ position, 0.0f }, size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawQuad(position, size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawQuad({ position, 0.0f }, size, texture, tileFactor, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawQuad(position, size, texture, tileFactor, tint);
	}

	// -- Draw Rotated Quad --

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Color& color)
	{
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size }, s_Data.WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Color& color)
	{
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), { size, size }, s_Data.WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Color& color)
	{
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, s_Data.WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Color& color)
	{
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), size, s_Data.WhiteTexture, { 1.0f, 1.0f }, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), size, texture, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), size, texture, tileFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), size, texture, { 1.0f, 1.0f }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawRotatedQuad({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, tileFactor, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawRotatedQuad(position, glm::radians(rotationDegrees), size, texture, tileFactor, tint);
	}

}
