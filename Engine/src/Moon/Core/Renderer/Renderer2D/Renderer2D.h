#pragma once

#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicCamera.h"


namespace Moon {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// ---- Primitives ----
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

	private:
		static void Super_DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

	};

}
