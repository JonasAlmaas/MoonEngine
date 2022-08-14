#include "mepch.h"
#include "Moon/Core/Renderer.h"

#include "Moon/Renderer/Renderer2D.h"


namespace Moon {
	// TODO: Rewrite everything here, this is very outdated!

	Ref<Renderer::SceneData> Renderer::s_SceneData = CreateRef<Renderer::SceneData>();
	
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

	// This does not do anything at the moment. Major rewrite of the while renderer is due.
	void Renderer::BeginScene(const Ref<RenderCamera>& renderCamera)
	{
		ME_PROFILE_FUNCTION();

		s_SceneData->ViewProjectionMatrix = renderCamera->GetViewProjection();
	}
	
	void Renderer::EndScene()
	{
		ME_PROFILE_FUNCTION();
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		ME_PROFILE_RENDERER_FUNCTION()

		shader->Bind();
		//shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
