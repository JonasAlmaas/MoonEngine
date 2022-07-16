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

	struct SpriteVertex
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
		static const uint32_t MaxSprites = 20000;
		static const uint32_t MaxVertices = MaxSprites * 4;
		static const uint32_t MaxIndices = MaxSprites * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		Ref<VertexArray> SpriteVertexArray;
		Ref<VertexBuffer> SpriteVertexBuffer;
		Ref<Shader> SpriteShader;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t SpriteIndexCount = 0;
		SpriteVertex* SpriteVertexBufferBase = nullptr;
		SpriteVertex* SpriteVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		Ref<Texture2D> WhiteTexture;
		Color WhiteColor;
		glm::vec2 DefaultTileFactor;

		glm::vec4 SpriteVertexPositions[4];
		glm::vec2 SpriteUVCoords[4];

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
		s_Data.SpriteShader = Shader::Create("Content/Shaders/Renderer2D_Sprite.glsl");

		int32_t samplers[Renderer2DData::MaxTextureSlots];
		for (uint32_t i = 0; i < Renderer2DData::MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.SpriteShader->Bind();
		s_Data.SpriteShader->SetIntArray("u_Textures", samplers, Renderer2DData::MaxTextureSlots);

		// Generate a 1x1 white texture
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t textureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&textureData, sizeof(uint32_t));

		// Set the white texture to slot 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		// Set up defaults
		s_Data.WhiteColor = Color(ColorFormat::RGBANormalized, 1.0f, 1.0f, 1.0f, 1.0f);
		s_Data.DefaultTileFactor = { 1.0f, 1.0f };

		// -- Sprite --
		s_Data.SpriteVertexArray = VertexArray::Create();

		s_Data.SpriteVertexBuffer = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(SpriteVertex));
		s_Data.SpriteVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "Position XYZ" },
			{ ShaderDataType::Float4, "Color" },
			{ ShaderDataType::Float2, "UV" },
			{ ShaderDataType::Float, "TextureIndex" },
			{ ShaderDataType::Int, "EntityID" },
		});

		s_Data.SpriteVertexArray->AddVertexBuffer(s_Data.SpriteVertexBuffer);

		s_Data.SpriteVertexBufferBase = new SpriteVertex[Renderer2DData::MaxVertices];

		uint32_t* spriteIndices = new uint32_t[Renderer2DData::MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < Renderer2DData::MaxIndices; i += 6)
		{
			spriteIndices[i + 0] = offset + 0;
			spriteIndices[i + 1] = offset + 1;
			spriteIndices[i + 2] = offset + 2;

			spriteIndices[i + 3] = offset + 2;
			spriteIndices[i + 4] = offset + 3;
			spriteIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> spriteIB = IndexBuffer::Create(spriteIndices, Renderer2DData::MaxIndices);
		s_Data.SpriteVertexArray->SetIndexBuffer(spriteIB);
		delete[] spriteIndices;

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
		s_Data.CircleVertexArray->SetIndexBuffer(spriteIB);	// Use Sprite IB

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

		// ---- Create basic sprite information -----
		s_Data.SpriteVertexPositions[0] = { -0.5, -0.5, 0, 1.0f };
		s_Data.SpriteVertexPositions[1] = { 0.5, -0.5, 0, 1.0f };
		s_Data.SpriteVertexPositions[2] = { 0.5,  0.5, 0, 1.0f };
		s_Data.SpriteVertexPositions[3] = { -0.5,  0.5, 0, 1.0f };

		s_Data.SpriteUVCoords[0] = { 0, 0 };
		s_Data.SpriteUVCoords[1] = { 1.0f, 0.0f };
		s_Data.SpriteUVCoords[2] = { 1.0f, 1.0f };
		s_Data.SpriteUVCoords[3] = { 0.0f, 1.0f };

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		ME_PROFILE_FUNCTION();

		delete[] s_Data.SpriteVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Ref<RenderCamera>& renderCamera)
	{
		ME_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = renderCamera->GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartSpriteBatch();
		StartCircleBatch();
		StartLineBatch();
	}

	void Renderer2D::EndScene()
	{
		ME_PROFILE_FUNCTION();

		FlushSpriteBatch();
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

	void Renderer2D::StartSpriteBatch()
	{
		s_Data.SpriteIndexCount = 0;
		s_Data.SpriteVertexBufferPtr = s_Data.SpriteVertexBufferBase;
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

	void Renderer2D::FlushSpriteBatch()
	{
		ME_PROFILE_FUNCTION();

		// Nothing to draw
		if (s_Data.SpriteIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.SpriteVertexBufferPtr - (uint8_t*)s_Data.SpriteVertexBufferBase);
		s_Data.SpriteVertexBuffer->SetData(s_Data.SpriteVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		s_Data.SpriteShader->Bind();
		RenderCommand::DrawIndexed(s_Data.SpriteVertexArray, s_Data.SpriteIndexCount);

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
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.SpriteVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.SpriteVertexPositions[i] * 2.0f;
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

	void Renderer2D::Super_DrawCircle(const glm::vec3& position, float radius, float thickness, float fade, const Color& color)
	{
		ME_PROFILE_FUNCTION();

		float diamater = radius * 2.0f;
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { diamater, diamater, 0.0f });
		glm::mat4 transform = translation * scale;

		Uber_DrawCircle(transform, thickness, fade, color);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, float thickness, float fade)
	{
		Uber_DrawCircle(transform, thickness, fade, s_Data.WhiteColor);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, float thickness, float fade, const Color& color)
	{
		Uber_DrawCircle(transform, thickness, fade, color);
	}

	void Renderer2D::DrawCircle(const glm::vec2& position, float radius, float thickness, float fade)
	{
		Super_DrawCircle({ position, 0.0f }, radius, thickness, fade, s_Data.WhiteColor);
	}

	void Renderer2D::DrawCircle(const glm::vec3& position, float radius, float thickness, float fade)
	{
		Super_DrawCircle(position, radius, thickness, fade, s_Data.WhiteColor);
	}

	void Renderer2D::DrawCircle(const glm::vec2& position, float radius, float thickness, float fade, const Color& color)
	{
		Super_DrawCircle({ position, 0.0f }, radius, thickness, fade, color);
	}

	void Renderer2D::DrawCircle(const glm::vec3& position, float radius, float thickness, float fade, const Color& color)
	{
		Super_DrawCircle(position, radius, thickness, fade, color);
	}

	// -- Line --

	void Renderer2D::Uber_DrawLine(const glm::vec3& p0, const glm::vec3& p1, const Color& color, int entityID)
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

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const Color& color)
	{
		Uber_DrawLine(p0, p1, color);
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	// -- Rectangle --

	void Renderer2D::Uber_DrawRect(const glm::mat4& transform, const Color& color, int entityID)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_Data.SpriteVertexPositions[i];

		Uber_DrawLine(lineVertices[0], lineVertices[1], color, entityID);
		Uber_DrawLine(lineVertices[1], lineVertices[2], color, entityID);
		Uber_DrawLine(lineVertices[2], lineVertices[3], color, entityID);
		Uber_DrawLine(lineVertices[3], lineVertices[0], color, entityID);
	}

	void Renderer2D::Super_DrawRect(const glm::vec3& position, const glm::vec2 size, const Color& color)
	{
		ME_PROFILE_FUNCTION();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size , 0.0f });
		glm::mat4 transform = translation * scale;

		Uber_DrawRect(transform, color);
	}

	void Renderer2D::Super_DrawRotatedRect(const glm::vec3& position, float rotationRadians, const glm::vec2 size, const Color& color)
	{
		ME_PROFILE_FUNCTION();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotationRadians, { 0.0f, 0.0f, 1.0f });
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size , 0.0f });
		glm::mat4 transform = translation * rotation * scale;

		Uber_DrawRect(transform, color);
	}

	// Draw Rectangle

	void Renderer2D::DrawRect(const glm::mat4& transform)
	{
		Uber_DrawRect(transform, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const Color& color)
	{
		Uber_DrawRect(transform, color);
	}

	void Renderer2D::DrawRect(const glm::vec2& position, float size)
	{
		Super_DrawRect({ position, 0.0f }, { size, size }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, float size)
	{
		Super_DrawRect(position, { size, size }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size)
	{
		Super_DrawRect({ position, 0.0f }, size, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size)
	{
		Super_DrawRect(position, size, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRect(const glm::vec2& position, float size, const Color& color)
	{
		Super_DrawRect({ position, 0.0f }, { size, size }, color);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, float size, const Color& color)
	{
		Super_DrawRect(position, { size, size }, color);
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
		Super_DrawRect({ position, 0.0f }, size, color);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const Color& color)
	{
		Super_DrawRect(position, size, color);
	}

	// Draw Rotated Rectangle

	void Renderer2D::DrawRotatedRect(const glm::vec2& position, float rotationDegrees, float size)
	{
		Super_DrawRotatedRect({ position, 0.0f }, glm::radians(rotationDegrees), { size, size }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedRect(const glm::vec3& position, float rotationDegrees, float size)
	{
		Super_DrawRotatedRect(position, glm::radians(rotationDegrees), { size, size }, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedRect(const glm::vec2& position, float rotationDegrees, const glm::vec2& size)
	{
		Super_DrawRotatedRect({ position, 0.0f }, glm::radians(rotationDegrees), size, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedRect(const glm::vec3& position, float rotationDegrees, const glm::vec2& size)
	{
		Super_DrawRotatedRect(position, glm::radians(rotationDegrees), size, s_Data.WhiteColor);
	}

	void Renderer2D::DrawRotatedRect(const glm::vec2& position, float rotationDegrees, float size, const Color& color)
	{
		Super_DrawRotatedRect({ position, 0.0f }, glm::radians(rotationDegrees), { size, size }, color);
	}

	void Renderer2D::DrawRotatedRect(const glm::vec3& position, float rotationDegrees, float size, const Color& color)
	{
		Super_DrawRotatedRect(position, glm::radians(rotationDegrees), { size, size }, color);
	}

	void Renderer2D::DrawRotatedRect(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Color& color)
	{
		Super_DrawRotatedRect({ position, 0.0f }, glm::radians(rotationDegrees), size, color);
	}

	void Renderer2D::DrawRotatedRect(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Color& color)
	{
		Super_DrawRotatedRect(position, glm::radians(rotationDegrees), size, color);
	}

	// -- Sprite --

	void Renderer2D::Uber_DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint, int entityID)
	{
		ME_PROFILE_FUNCTION();

		if (s_Data.SpriteIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushSpriteBatch();
			StartSpriteBatch();
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
			s_Data.SpriteVertexBufferPtr->Position = transform * s_Data.SpriteVertexPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = tint.Format == ColorFormat::RGBANormalized ? tint : tint.GetNormalized();
			s_Data.SpriteVertexBufferPtr->UV = tileFactor * s_Data.SpriteUVCoords[i];
			s_Data.SpriteVertexBufferPtr->TextureIndex = (float)textureIndex; // This is a float because it be like that some times...
			s_Data.SpriteVertexBufferPtr->EntityID = entityID;
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;

		#if ME_ENABLE_RENDERER2D_STATISTICS
			s_Data.Stats.SpriteCount++;
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

		if (s_Data.SpriteIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushSpriteBatch();
			StartSpriteBatch();
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
			s_Data.SpriteVertexBufferPtr->Position = transform * s_Data.SpriteVertexPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = tint.Format == ColorFormat::RGBANormalized ? tint : tint.GetNormalized();
			s_Data.SpriteVertexBufferPtr->UV = subTexture->GetUVCoords()[i];
			s_Data.SpriteVertexBufferPtr->TextureIndex = (float)textureIndex; // This is a float because it be like that some times...
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;

		#if ME_ENABLE_RENDERER2D_STATISTICS
			s_Data.Stats.SpriteCount++;
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
