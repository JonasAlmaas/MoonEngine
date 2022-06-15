#pragma once

#include "Moon/Camera/Orthographic/OrthographicCamera.h"
#include "Moon/Core/Renderer/Texture/Texture.h"
#include "Moon/Core/Type/Color.h"


namespace Moon {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// ---- Primitives ----

		static void DrawQuad(const glm::vec2& position, float size, const Color& color);
		static void DrawQuad(const glm::vec3& position, float size, const Color& color);

		static void DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, float tileFactor);
		static void DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, float tileFactor);

		static void DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);
		static void DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);

		static void DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const Color& tint);
		static void DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const Color& tint);

		static void DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);
		static void DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);

		static void DrawQuad(const glm::vec2& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);
		static void DrawQuad(const glm::vec3& position, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Color& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Color& color);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Color& color);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const Color& tint);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const Color& tint);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, float size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Color& color);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Color& color);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const Color& tint);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const Color& tint);

		static void DrawRotatedQuad(const glm::vec2& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);
		static void DrawRotatedQuad(const glm::vec3& position, float rotationDegrees, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

	private:
		static void Super_DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);
		static void Super_DrawRotatedQuad(const glm::vec3& position, float rotationRadians, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tileFactor, const Color& tint);

	};

}
