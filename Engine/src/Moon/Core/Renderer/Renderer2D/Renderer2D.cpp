#include "mepch.h"
#include "Moon/Core/Renderer/Renderer2D/Renderer2D.h"

#include "Moon/Core/Renderer/IndexBuffer/IndexBuffer.h"
#include "Moon/Core/Renderer/RenderCommand/RenderCommand.h"
#include "Moon/Core/Renderer/VertexArray/VertexArray.h"
#include "Moon/Core/Renderer/VertexBuffer/VertexBuffer.h"
#include "Moon/Core/Renderer/Shader/Shader.h"

// TODO: REMOVE!
#include "Moon/Platform/OpenGL/Shader/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Moon {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> Shader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::Create();

		float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		Ref<VertexBuffer> VB = VertexBuffer::Create(vertices, sizeof(vertices));

		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "Position XYZ" },
			{ ShaderDataType::Float2, "UV" },
		};
		VB->SetLayout(layout);
		s_Data->QuadVertexArray->AddVertexBuffer(VB);

		uint32_t indices[3 * 2] = {
			0, 1, 2,
			0, 2, 3
		};
		Ref<IndexBuffer> IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		s_Data->QuadVertexArray->SetIndexBuffer(IB);

		s_Data->Shader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->Shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		//m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{
	}

	// ---- Primitives ----

	void Renderer2D::Super_DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->Shader->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformFloat3("u_Color", color);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size , 0 });
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
	{
		Super_DrawQuad({ position, 0 }, size, { color, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		Super_DrawQuad({ position, 0 }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& color)
	{
		Super_DrawQuad(position, size, { color, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		Super_DrawQuad(position, size, color);
	}

}