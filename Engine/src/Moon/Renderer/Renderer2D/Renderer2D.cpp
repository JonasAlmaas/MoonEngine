#include "mepch.h"
#include "Moon/Renderer/Renderer2D/Renderer2D.h"

#include "Moon/Core/Renderer/Buffer/IndexBuffer/IndexBuffer.h"
#include "Moon/Core/Renderer/Buffer/UniformBuffer/UniformBuffer.h"
#include "Moon/Core/Renderer/Buffer/VertexBuffer/VertexBuffer.h"
#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"
#include "Moon/Core/Renderer/Shader/Shader.h"
#include "Moon/Core/Renderer/Texture/Texture.h"
#include "Moon/Core/Renderer/VertexArray/VertexArray.h"


namespace Moon {

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor only
		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor only
		int EntityID;
	};

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 UV;
		float TextureIndex;

		// Editor only
		int EntityID;
	};

	struct Renderer2DData
	{
		// Max per drawcall
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		Ref<Texture2D> WhiteTexture;
		Color WhiteColor;
		glm::vec2 DefaultTileFactor;

		glm::vec4 QuadVertexPositions[4];
		glm::vec2 QuadUVCoords[4];

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = White texture

		#if ME_ENABLE_RENDERER2D_STATISTICS
			Renderer2D::Statistics Stats;
		#endif

		// Uniform buffer data to be sent to the gpu
		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		ME_PROFILE_FUNCTION();

		s_Data.CircleShader = Shader::Create("Content/Shaders/Renderer2D_Circle.glsl");
		s_Data.LineShader = Shader::Create("Content/Shaders/Renderer2D_Line.glsl");
		s_Data.QuadShader = Shader::Create("Content/Shaders/Renderer2D_Quad.glsl");

		int32_t samplers[Renderer2DData::MaxTextureSlots];
		for (uint32_t i = 0; i < Renderer2DData::MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetIntArray("u_Textures", samplers, Renderer2DData::MaxTextureSlots);

		// Generate a 1x1 white texture
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t textureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&textureData, sizeof(uint32_t));

		// Set the white texture to slot 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		// Set up defaults
		s_Data.WhiteColor = Color(ColorFormat::RGBANormalized, 1.0f, 1.0f, 1.0f, 1.0f);
		s_Data.DefaultTileFactor = { 1.0f, 1.0f };

		// -- Quad --
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "Position XYZ" },
			{ ShaderDataType::Float4, "Color" },
			{ ShaderDataType::Float2, "UV" },
			{ ShaderDataType::Float, "TextureIndex" },
			{ ShaderDataType::Int, "EntityID" },
		});

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[Renderer2DData::MaxVertices];

		uint32_t* quadIndices = new uint32_t[Renderer2DData::MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < Renderer2DData::MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, Renderer2DData::MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// -- Circle --
		s_Data.CircleVertexArray = VertexArray::Create();

		s_Data.CircleVertexBuffer = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "World Position XYZ" },
			{ ShaderDataType::Float3, "Local Position XYZ" },
			{ ShaderDataType::Float4, "Color" },
			{ ShaderDataType::Float, "Thickness" },
			{ ShaderDataType::Float, "Fade" },
			{ ShaderDataType::Int, "EntityID" },
		});

		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexBufferBase = new CircleVertex[Renderer2DData::MaxVertices];
		s_Data.CircleVertexArray->SetIndexBuffer(quadIB);	// Use Quad IB

		// -- Line --
		s_Data.LineVertexArray = VertexArray::Create();

		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float4, "Color" },
			{ ShaderDataType::Int, "a_EntityID" }
		});

		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

		// ---- Create basic quad information -----
		s_Data.QuadVertexPositions[0] = { -0.5, -0.5, 0, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5, -0.5, 0, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5,  0.5, 0, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5,  0.5, 0, 1.0f };

		s_Data.QuadUVCoords[0] = { 0, 0 };
		s_Data.QuadUVCoords[1] = { 1.0f, 0.0f };
		s_Data.QuadUVCoords[2] = { 1.0f, 1.0f };
		s_Data.QuadUVCoords[3] = { 0.0f, 1.0f };

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		ME_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const glm::mat4& viewProjectionMatrix)
	{
		ME_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = viewProjectionMatrix;
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartQuadBatch();
		StartCircleBatch();
		StartLineBatch();
	}

	void Renderer2D::BeginScene(const glm::mat4& cameraProj, const glm::mat4& transform)
	{
		ME_PROFILE_FUNCTION();

		glm::mat4 viewProjectionMatrix = cameraProj * glm::inverse(transform);
		BeginScene(viewProjectionMatrix);
	}

	void Renderer2D::EndScene()
	{
		ME_PROFILE_FUNCTION();

		FlushQuadBatch();
		FlushCircleBatch();
		FlushLineBatch();
	}

	// -- Start Batches --

	void Renderer2D::StartCircleBatch()
	{
		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
	}

	void Renderer2D::StartLineBatch()
	{
		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
	}

	void Renderer2D::StartQuadBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	// -- Flush Batches --

	void Renderer2D::FlushCircleBatch()
	{
		ME_PROFILE_FUNCTION();

		// Nothing to draw
		if (s_Data.CircleIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
		s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

		s_Data.CircleShader->Bind();
		RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);

		#if ME_ENABLE_RENDERER2D_STATISTICS
			s_Data.Stats.DrawCalls++;
		#endif
	}

	void Renderer2D::FlushLineBatch()
	{
		ME_PROFILE_FUNCTION();

		// Nothing to draw
		if (s_Data.LineVertexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
		s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

		s_Data.LineShader->Bind();
		RenderCommand::SetLineWidth(s_Data.LineWidth);
		RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);

		#if ME_ENABLE_RENDERER2D_STATISTICS
			s_Data.Stats.DrawCalls++;
		#endif
	}

	void Renderer2D::FlushQuadBatch()
	{
		ME_PROFILE_FUNCTION();

		// Nothing to draw
		if (s_Data.QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		s_Data.QuadShader->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

		#if ME_ENABLE_RENDERER2D_STATISTICS
			s_Data.Stats.DrawCalls++;
		#endif
	}

	// -- Stats --

	#if ME_ENABLE_RENDERER2D_STATISTICS
		void Renderer2D::ResetStats()
		{
			memset(&s_Data.Stats, 0, sizeof(Statistics));
		}

		Renderer2D::Statistics Renderer2D::GetStats()
		{
			return s_Data.Stats;
		}
	#endif

	// -- Draw Renderer Components --

	void Renderer2D::DrawCircleRendererComponent(const glm::mat4& transform, CircleRendererComponent& component, int entityID)
	{
		Uber_DrawCircle(transform, component.Thickness, component.Fade, component.Color, entityID);
	}

	void Renderer2D::DrawSpriteRendererComponent(const glm::mat4& transform, SpriteRendererComponent& component, int entityID)
	{
		if (component.Texture)
			Uber_DrawSprite(transform, component.Texture, component.TileFactor, component.Color, entityID);
		else
			Uber_DrawSprite(transform, s_Data.WhiteTexture, s_Data.DefaultTileFactor, component.Color, entityID);
	}

	// -- Circle --

	void Renderer2D::Uber_DrawCircle(const glm::mat4& transform, float thickness, float fade, const Color& color, int entityID)
	{
		ME_PROFILE_FUNCTION();

		if (s_Data.CircleIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushCircleBatch();
			StartCircleBatch();
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->Color = color.Format == ColorFormat::RGBANormalized ? color : color.GetNormalized();
			s_Data.CircleVertexBufferPtr->EntityID = entityID;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;

		#if ME_ENABLE_RENDERER2D_STATISTICS
			s_Data.Stats.CircleCount++;
		#endif
	}

	//void Renderer2D::DrawCircle(const glm::mat4& transform, float thickness, float fade, const Color& color)
	//{
	//}

	//void Renderer2D::DrawCircle(float radius, float thickness, float fade, const Color& color)
	//{
	//}

	// -- Line --

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const Color& color, int entityID)
	{
		ME_PROFILE_FUNCTION();

		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;

		#if ME_ENABLE_RENDERER2D_STATISTICS
			s_Data.Stats.LineCount++;
		#endif
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	// -- Sprite --

	void Renderer2D::Uber_DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint, int entityID)
	{
		ME_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushQuadBatch();
			StartQuadBatch();
		}

		int textureIndex = 0;

		// Check that the texture isn't the white texture.
		if (*s_Data.TextureSlots[0] != *texture)
		{
			// Get texture index, only if it has been added previusly.
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i] == *texture)
				{
					textureIndex = i;
					break;
				}
			}

			// If the texture isn't in a texture slot, add it.
			if (textureIndex == 0)
			{
				textureIndex = s_Data.TextureSlotIndex;
				s_Data.TextureSlots[textureIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tint.Format == ColorFormat::RGBANormalized ? tint : tint.GetNormalized();
			s_Data.QuadVertexBufferPtr->UV = tileFactor * s_Data.QuadUVCoords[i];
			s_Data.QuadVertexBufferPtr->TextureIndex = (float)textureIndex; // This is a float because it be like that some times...
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		#if ME_ENABLE_RENDERER2D_STATISTICS
			s_Data.Stats.QuadCount++;
		#endif
	}

	void Renderer2D::Super_DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		ME_PROFILE_FUNCTION();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size , 0.0f });
		glm::mat4 transform = translation * scale;

		Uber_DrawSprite(transform, texture, tileFactor, tint);
	}

	void Renderer2D::Super_DrawRotatedSprite(const glm::vec3& position, float rotationRadians, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		ME_PROFILE_FUNCTION();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotationRadians, { 0.0f, 0.0f, 1.0f });
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size , 0.0f });
		glm::mat4 transform = translation * rotation * scale;

		Uber_DrawSprite(transform, texture, tileFactor, tint);
	}
	
	// Draw Sprite Using SubTexture

	void Renderer2D::Uber_DrawSprite(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		ME_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushQuadBatch();
			StartQuadBatch();
		}

		int textureIndex = 0;

		// Check that the texture isn't the white texture.
		if (*s_Data.TextureSlots[0] != *subTexture->GetTexture())
		{
			// Get texture index, only if it has been added previusly.
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i] == *subTexture->GetTexture())
				{
					textureIndex = i;
					break;
				}
			}

			// If the texture isn't in a texture slot, add it.
			if (textureIndex == 0)
			{
				textureIndex = s_Data.TextureSlotIndex;
				s_Data.TextureSlots[textureIndex] = subTexture->GetTexture();
				s_Data.TextureSlotIndex++;
			}
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tint.Format == ColorFormat::RGBANormalized ? tint : tint.GetNormalized();
			s_Data.QuadVertexBufferPtr->UV = subTexture->GetUVCoords()[i];
			s_Data.QuadVertexBufferPtr->TextureIndex = (float)textureIndex; // This is a float because it be like that some times...
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		#if ME_ENABLE_RENDERER2D_STATISTICS
			s_Data.Stats.QuadCount++;
		#endif
	}

	void Renderer2D::Super_DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		ME_PROFILE_FUNCTION();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size , 0.0f });
		glm::mat4 transform = translation * scale;

		Uber_DrawSprite(transform, subTexture, tint);
	}

	void Renderer2D::Super_DrawRotatedSprite(const glm::vec3& position, float rotationRadians, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		ME_PROFILE_FUNCTION();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotationRadians, { 0.0f, 0.0f, 1.0f });
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size , 0.0f });
		glm::mat4 transform = translation * rotation * scale;

		Uber_DrawSprite(transform, subTexture, tint);
	}

	// Draw sprite

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Color& color)
	{
		Uber_DrawSprite(transform, s_Data.WhiteTexture, s_Data.DefaultTileFactor, color);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture)
	{
		Uber_DrawSprite(transform, texture, s_Data.DefaultTileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, float tileFactor)
	{
		Uber_DrawSprite(transform, texture, { tileFactor, tileFactor }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Uber_DrawSprite(transform, texture, tileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, const Color& tint)
	{
		Uber_DrawSprite(transform, texture, s_Data.DefaultTileFactor, tint);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Uber_DrawSprite(transform, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Uber_DrawSprite(transform, texture, tileFactor, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, float size, const Color& color)
	{
		Super_DrawSprite({ position, 0.0f }, { size, size }, s_Data.WhiteTexture, s_Data.DefaultTileFactor, color);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, float size, const Color& color)
	{
		Super_DrawSprite(position, { size, size }, s_Data.WhiteTexture, s_Data.DefaultTileFactor, color);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite({ position, 0.0f }, { size, size * aspectRatio }, texture, s_Data.DefaultTileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite(position, { size, size * aspectRatio }, texture, s_Data.DefaultTileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite({ position, 0.0f }, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite(position, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite({ position, 0.0f }, { size, size * aspectRatio }, texture, tileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite(position, { size, size * aspectRatio }, texture, tileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite({ position, 0.0f }, { size, size * aspectRatio }, texture, s_Data.DefaultTileFactor, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite(position, { size, size * aspectRatio }, texture, s_Data.DefaultTileFactor, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite({ position, 0.0f }, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite(position, { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite({ position, 0.0f }, { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawSprite(position, { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
		Super_DrawSprite({ position, 0.0f }, size, s_Data.WhiteTexture, s_Data.DefaultTileFactor, color);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec2& size, const Color& color)
	{
		Super_DrawSprite(position, size, s_Data.WhiteTexture, s_Data.DefaultTileFactor, color);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawSprite({ position, 0.0f }, size, texture, s_Data.DefaultTileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawSprite(position, size, texture, s_Data.DefaultTileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawSprite({ position, 0.0f }, size, texture, { tileFactor, tileFactor }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawSprite(position, size, texture, { tileFactor, tileFactor }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawSprite({ position, 0.0f }, size, texture, tileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawSprite(position, size, texture, tileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawSprite({ position, 0.0f }, size, texture, s_Data.DefaultTileFactor, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawSprite(position, size, texture, s_Data.DefaultTileFactor, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawSprite({ position, 0.0f }, size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawSprite(position, size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawSprite({ position, 0.0f }, size, texture, tileFactor, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawSprite(position, size, texture, tileFactor, tint);
	}

	// Draw Rotated Sprte

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Color& color)
	{
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), { size, size }, s_Data.WhiteTexture, s_Data.DefaultTileFactor, color);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Color& color)
	{
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), { size, size }, s_Data.WhiteTexture, s_Data.DefaultTileFactor, color);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, s_Data.DefaultTileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, s_Data.DefaultTileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, s_Data.DefaultTileFactor, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, s_Data.DefaultTileFactor, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		float aspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), { size, size * aspectRatio }, texture, tileFactor, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Color& color)
	{
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), size, s_Data.WhiteTexture, s_Data.DefaultTileFactor, color);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Color& color)
	{
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), size, s_Data.WhiteTexture, s_Data.DefaultTileFactor, color);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, s_Data.DefaultTileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), size, texture, s_Data.DefaultTileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor)
	{
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, tileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor)
	{
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), size, texture, tileFactor, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, s_Data.DefaultTileFactor, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint)
	{
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), size, texture, s_Data.DefaultTileFactor, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint)
	{
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), size, texture, { tileFactor, tileFactor }, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), size, texture, tileFactor, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint)
	{
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), size, texture, tileFactor, tint);
	}

	// -- Draw Sprite Using SubTexture --

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture)
	{
		Uber_DrawSprite(transform, subTexture, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		Uber_DrawSprite(transform, subTexture, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, float size, const Ref<SubTexture2D>& subTexture)
	{
		glm::vec2 spriteSize = subTexture->GetSpriteSize();
		Super_DrawSprite({ position, 0.0f }, { size * spriteSize.x , size * spriteSize.y }, subTexture, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, float size, const Ref<SubTexture2D>& subTexture)
	{
		glm::vec2 spriteSize = subTexture->GetSpriteSize();
		Super_DrawSprite(position, { size * spriteSize.x , size * spriteSize.y }, subTexture, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, float size, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		glm::vec2 spriteSize = subTexture->GetSpriteSize();
		Super_DrawSprite({ position, 0.0f }, { size * spriteSize.x , size * spriteSize.y }, subTexture, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, float size, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		glm::vec2 spriteSize = subTexture->GetSpriteSize();
		Super_DrawSprite(position, { size * spriteSize.x , size * spriteSize.y }, subTexture, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture)
	{
		Super_DrawSprite({ position, 0.0f }, size, subTexture, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture)
	{
		Super_DrawSprite(position, size, subTexture, s_Data.WhiteColor);
	}

	void Renderer2D::DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		Super_DrawSprite({ position, 0.0f }, size, subTexture, tint);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		Super_DrawSprite(position, size, subTexture, tint);
	}

	// Draw Rotated Sprite Using SubTexture

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<SubTexture2D>& subTexture)
	{
		glm::vec2 spriteSize = subTexture->GetSpriteSize();
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), { size * spriteSize.x , size * spriteSize.y }, subTexture, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<SubTexture2D>& subTexture)
	{
		glm::vec2 spriteSize = subTexture->GetSpriteSize();
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), { size * spriteSize.x , size * spriteSize.y }, subTexture, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		glm::vec2 spriteSize = subTexture->GetSpriteSize();
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), { size * spriteSize.x , size * spriteSize.y }, subTexture, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		glm::vec2 spriteSize = subTexture->GetSpriteSize();
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), { size * spriteSize.x , size * spriteSize.y }, subTexture, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<SubTexture2D>& subTexture)
	{
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), size, subTexture, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<SubTexture2D>& subTexture)
	{
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), size, subTexture, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		Super_DrawRotatedSprite({ position, 0.0f }, glm::radians(rotationDegrees), size, subTexture, tint);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint)
	{
		Super_DrawRotatedSprite(position, glm::radians(rotationDegrees), size, subTexture, tint);
	}

}
