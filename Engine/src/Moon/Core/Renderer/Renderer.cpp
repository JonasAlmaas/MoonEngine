#include "mepch.h"
#include "Moon/Core/Renderer/Renderer.h"

#include "Moon/Core/Renderer/Renderer2D/Renderer2D.h"


namespace Moon {

	Ref<Renderer::SceneData> Renderer::m_SceneData = CreateRef<Renderer::SceneData>();
	
	void Renderer::Init()
	{
		ME_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		ME_PROFILE_FUNCTION();

		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		ME_PROFILE_FUNCTION();

		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	
	void Renderer::EndScene()
	{
		ME_PROFILE_FUNCTION();
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		ME_PROFILE_RENDERER_FUNCTION()

		shader->Bind();
		shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
