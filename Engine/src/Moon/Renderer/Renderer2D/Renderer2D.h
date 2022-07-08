#pragma once

#include "Moon/Core/Renderer/Camera/Camera.h"
#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicCamera.h"
#include "Moon/Core/Renderer/Texture/Texture.h"
#include "Moon/Renderer/Texture/SubTexture2D.h"
#include "Moon/Scene/Component/SpriteRendererComponent.h"


namespace Moon {

	class Renderer2D
	{
	public:
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const glm::mat4& viewProjectionMatrix);
		static void BeginScene(const glm::mat4& cameraProj, const glm::mat4& transform);

		static void EndScene();

		#if ME_ENABLE_RENDERER2D_STATISTICS
			static void ResetStats();
			static Statistics GetStats();
		#else
			static void ResetStats() {};
			static Statistics GetStats() { return Statistics(); }
		#endif

	private:
		static void StartBatch();
		static void FlushBatch();

		// ---- Primitives ----

		static void Uber_DrawSprite(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint, int entityID = -1);
		static void Super_DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);
		static void Super_DrawRotatedSprite(const glm::vec3& position, float rotationRadians, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

		static void Uber_DrawSprite(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, const Color& tint);
		static void Super_DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint);
		static void Super_DrawRotatedSprite(const glm::vec3& position, float rotationRadians, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const Color& tint);

	public:
		static void DrawSpriteComponent(const glm::mat4& transform, SpriteRendererComponent& spriteComponent, int entityID);

		/*
		 * @brief If you try to define a tile factor as a vec2 it might be misstaken for a Color.
		 * To prevent this, define it as glm::vec2(x, y) instead of { x, y }.
		 */
		static void DrawSprite() {};

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
