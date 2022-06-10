#include "mepch.h"
#include "Moon/Core/Renderer/Renderer.h"

#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"


namespace Moon {
	
	void Renderer::BeginScene()
	{
	}
	
	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
