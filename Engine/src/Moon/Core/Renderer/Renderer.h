#pragma once

#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicCamera.h"
#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"
#include "Moon/Core/Renderer/Shader/Shader.h"


namespace Moon {

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}
