#include "mepch.h"
#include "Moon/Core/Renderer.h"

#include "Moon/Core/Renderer/Buffer/UniformBuffer.h"
#include "Moon/Renderer/Renderer2D.h"


namespace Moon {

	struct RendererData
	{
		Ref<Shader> TextShader;

		// -- Uniform buffers --
		struct CameraData
		{
			glm::mat4 ViewProjection;
		};

		struct TransformData
		{
			glm::mat4 Transform;
		};

		CameraData CameraBuffer;
		TransformData TransformBuffer;

		Ref<UniformBuffer> CameraUniformBuffer;
		Ref<UniformBuffer> TransformUniformBuffer;
	};

	static RendererData* s_Data = nullptr;
	
	void Renderer::Init()
	{
		ME_PROFILE_FUNCTION();

		RenderCommand::Init();

		s_Data = new RendererData();

		s_Data->TextShader = Shader::Create("Content/Shaders/Core/Text.glsl");

		s_Data->CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
		s_Data->TransformUniformBuffer = UniformBuffer::Create(sizeof(RendererData::TransformData), 1);

		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		ME_PROFILE_FUNCTION();

		Renderer2D::Shutdown();

		delete s_Data;
		s_Data = nullptr;
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Ref<RenderCamera>& renderCamera)
	{
		ME_PROFILE_FUNCTION();

		s_Data->CameraBuffer.ViewProjection = renderCamera->GetViewProjection();
		s_Data->CameraUniformBuffer->SetData(&s_Data->CameraBuffer, sizeof(RendererData::CameraData));
	}
	
	void Renderer::EndScene()
	{
		ME_PROFILE_FUNCTION();
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		ME_PROFILE_RENDERER_FUNCTION()

		s_Data->TransformBuffer.Transform = transform;
		s_Data->TransformUniformBuffer->SetData(&s_Data->TransformBuffer, sizeof(RendererData::TransformData));

		shader->Bind();
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::RenderTextSurface(const TextSurface& textSurface, const glm::mat4& transform)
	{
		ME_PROFILE_RENDERER_FUNCTION()

		s_Data->TransformBuffer.Transform = transform;
		s_Data->TransformUniformBuffer->SetData(&s_Data->TransformBuffer, sizeof(RendererData::TransformData));

		s_Data->TextShader->Bind();

		textSurface.GetFont()->GetFontAtlas()->Bind();

		Ref<VertexArray> va = textSurface.GetVertexArray();
		va->Bind();

		RenderCommand::SetDepthTest(false);
		RenderCommand::DrawIndexed(va);
	}

	void Renderer::RenderTextSurface(const TextSurface& textSurface, const glm::vec3& position, const glm::vec3& rotation)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(glm::quat(rotation));
		RenderTextSurface(textSurface, transform);
	}

	void Renderer::RenderTextSurface(const TextSurface& textSurface, const glm::vec2& position, float rotationRadians)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position, 0.0, }) * glm::rotate(glm::mat4(1.0f), rotationRadians, { 0.0f, 0.0f, 1.0f });
		RenderTextSurface(textSurface, transform);
	}

	void Renderer::RenderTextSurface(const TextSurface& textSurface, const glm::vec3& position)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		RenderTextSurface(textSurface, transform);
	}

	void Renderer::RenderTextSurface(const TextSurface& textSurface, const glm::vec2& position)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position, 0.0, });
		RenderTextSurface(textSurface, transform);
	}

}
