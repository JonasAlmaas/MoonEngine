#pragma once

#include "Moon/Core/Renderer/RenderCamera.h"
#include "Moon/Core/Renderer/RenderCommand.h"
#include "Moon/Core/Renderer/Shader.h"
#include "Moon/Renderer/TextSurface.h"


namespace Moon {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Ref<RenderCamera>& renderCamera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		// NOTE: This disables depth testing
		static void RenderTextSurface(const TextSurface& textSurface, const glm::mat4& transform);
		static void RenderTextSurface(const TextSurface& textSurface, const glm::vec3& position, const glm::vec3& rotation);
		static void RenderTextSurface(const TextSurface& textSurface, const glm::vec2& position, float rotationRadians);
		static void RenderTextSurface(const TextSurface& textSurface, const glm::vec3& position);
		static void RenderTextSurface(const TextSurface& textSurface, const glm::vec2& position);

	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	};

}
