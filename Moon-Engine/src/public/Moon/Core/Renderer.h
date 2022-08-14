#pragma once

#include "Moon/Core/Renderer/RenderCamera.h"
#include "Moon/Core/Renderer/RenderCommand.h"
#include "Moon/Core/Renderer/Shader.h"


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

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Ref<SceneData> s_SceneData;
	};

}
