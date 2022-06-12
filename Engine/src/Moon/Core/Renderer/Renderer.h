#pragma once

#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicCamera.h"
#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"
#include "Moon/Core/Renderer/Shader/Shader.h"


namespace Moon {

	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Ref<SceneData> m_SceneData;
	};

}
