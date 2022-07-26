#pragma once

#include "Moon/Core/Renderer/RenderCamera.h"
#include "Moon/Core/Renderer/Texture.h"
#include "Moon/Renderer/SubTexture2D.h"
#include "Moon/Scene/Component/Renderer/CircleRendererComponent.h"
#include "Moon/Scene/Component/Renderer/SpriteRendererComponent.h"


namespace Moon {

	class Renderer2D
	{
	public:
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t CircleCount = 0;
			uint32_t LineCount = 0;
			uint32_t SpriteCount = 0;

			uint32_t GetTotalVertexCount() const
			{
				uint32_t circleVertices = CircleCount * 4;
				uint32_t linesVertices = LineCount * 2;
				uint32_t spriteVertices = SpriteCount * 4;
				return linesVertices + circleVertices + spriteVertices;
			}

			uint32_t GetTotalIndexCount() const
			{
				uint32_t circleIndices = CircleCount * 6;
				uint32_t spriteIndices = SpriteCount * 6;
				return circleIndices + spriteIndices;
			}
		};

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Ref<RenderCamera>& renderCamera);
		static void EndScene();

		#if ME_ENABLE_RENDERER2D_STATISTICS
			static void ResetStats();
			static Statistics GetStats();
		#else
			static void ResetStats() {};
			static Statistics GetStats() { return Statistics(); }
		#endif

	private:
		static void StartCircleBatch();
		static void StartLineBatch();
		static void StartSpriteBatch();

		static void FlushCircleBatch();
		static void FlushLineBatch();
		static void FlushSpriteBatch();

		// ---- Primitives ----

		// -- Circle --
		static void Uber_DrawCircle(const glm::mat4& transform, float thickness, float fade, const Color& color, int entityID = -1);
		static void Super_DrawCircle(const glm::vec3& position, float radius, float thickness, float fade, const Color& color);

		// -- Line --
		static void Uber_DrawLine(const glm::vec3& p0, const glm::vec3& p1, const Color& color, int entityID = -1);

		// -- Rectangle --
		static void Uber_DrawRect(const glm::mat4& transform, const Color& color, int entityID = -1);
		static void Super_DrawRect(const glm::vec3& position, const glm::vec2 size, const Color& color);
		static void Super_DrawRotatedRect(const glm::vec3& position, float rotationRadians, const glm::vec2 size, const Color& color);

		// -- Sprite --
		static void Uber_DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint, int entityID = -1);
		static void Super_DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);
		static void Super_DrawRotatedSprite(const glm::vec3& position, float rotationRadians, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

		static void Uber_DrawSprite(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, const Color& tint);
		static void Super_DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint);
		static void Super_DrawRotatedSprite(const glm::vec3& position, float rotationRadians, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint);

	public:
		// -- Draw Renderer Components --
		static void DrawCircleRendererComponent(const glm::mat4& transform, CircleRendererComponent& component, int entityID);
		static void DrawSpriteRendererComponent(const glm::mat4& transform, SpriteRendererComponent& component, int entityID);

		// -- Circle --
		static void DrawCircle(const glm::mat4& transform, float thickness, float fade);
		static void DrawCircle(const glm::mat4& transform, float thickness, float fade, const Color& color);

		static void DrawCircle(const glm::vec2& position, float radius, float thickness, float fade);
		static void DrawCircle(const glm::vec3& position, float radius, float thickness, float fade);

		static void DrawCircle(const glm::vec2& position, float radius, float thickness, float fade, const Color& color);
		static void DrawCircle(const glm::vec3& position, float radius, float thickness, float fade, const Color& color);

		// -- Line --
		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const Color& color);

		static float GetLineWidth();
		static void SetLineWidth(float width);

		// -- Rectangle --
		static void DrawRect(const glm::mat4& transform);
		static void DrawRect(const glm::mat4& transform, const Color& color);

		static void DrawRect(const glm::vec2& position, float size);
		static void DrawRect(const glm::vec3& position, float size);

		static void DrawRect(const glm::vec2& position, const glm::vec2& size);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size);

		static void DrawRect(const glm::vec2& position, float size, const Color& color);
		static void DrawRect(const glm::vec3& position, float size, const Color& color);

		static void DrawRect(const glm::vec2& position, const glm::vec2& size, const Color& color);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const Color& color);

		static void DrawRotatedRect(const glm::vec2& position, float rotationDegrees, float size);
		static void DrawRotatedRect(const glm::vec3& position, float rotationDegrees, float size);

		static void DrawRotatedRect(const glm::vec2& position, float rotationDegrees, const glm::vec2& size);
		static void DrawRotatedRect(const glm::vec3& position, float rotationDegrees, const glm::vec2& size);

		static void DrawRotatedRect(const glm::vec2& position, float rotationDegrees, float size, const Color& color);
		static void DrawRotatedRect(const glm::vec3& position, float rotationDegrees, float size, const Color& color);

		static void DrawRotatedRect(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Color& color);
		static void DrawRotatedRect(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Color& color);

		// -- Sprite --
		static void DrawSprite(const glm::mat4& transform, const Color& color);
		static void DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture);
		static void DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, float tileFactor);
		static void DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);
		static void DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, const Color& tint);
		static void DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);
		static void DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

		static void DrawSprite(const glm::vec2& position, float size, const Color& color);
		static void DrawSprite(const glm::vec3& position, float size, const Color& color);

		static void DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture);
		static void DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture);

		static void DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture, float tileFactor);
		static void DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture, float tileFactor);

		static void DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);
		static void DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);

		static void DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const Color& tint);
		static void DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const Color& tint);

		static void DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);
		static void DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);

		static void DrawSprite(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);
		static void DrawSprite(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

		static void DrawSprite(const glm::vec2& position, const glm::vec2& size, const Color& color);
		static void DrawSprite(const glm::vec3& position, const glm::vec2& size, const Color& color);

		static void DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor);
		static void DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor);

		static void DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);
		static void DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);

		static void DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint);
		static void DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint);

		static void DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);
		static void DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);

		static void DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);
		static void DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Color& color);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Color& color);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const Color& tint);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const Color& tint);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Color& color);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Color& color);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

		// -- Using SubTexture --
		static void DrawSprite(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture);
		static void DrawSprite(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, const Color& tint);

		static void DrawSprite(const glm::vec2& position, float size, const Ref<SubTexture2D>& subTexture);
		static void DrawSprite(const glm::vec3& position, float size, const Ref<SubTexture2D>& subTexture);

		static void DrawSprite(const glm::vec2& position, float size, const Ref<SubTexture2D>& subTexture, const Color& tint);
		static void DrawSprite(const glm::vec3& position, float size, const Ref<SubTexture2D>& subTexture, const Color& tint);

		static void DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture);
		static void DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture);

		static void DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint);
		static void DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<SubTexture2D>& subTexture);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<SubTexture2D>& subTexture);
		
		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, float size, const Ref<SubTexture2D>& subTexture, const Color& tint);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, float size, const Ref<SubTexture2D>& subTexture, const Color& tint);
		
		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<SubTexture2D>& subTexture);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<SubTexture2D>& subTexture);

		static void DrawRotatedSprite(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint);
		static void DrawRotatedSprite(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint);

	};

}
