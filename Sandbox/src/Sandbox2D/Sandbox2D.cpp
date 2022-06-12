#include "Sandbox2D/Sandbox2D.h"

#include <Moon/Platform/OpenGL/Shader/OpenGLShader.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), false, true, true)
{

}

void Sandbox2D::OnAttach()
{
	m_VertexArray = VertexArray::Create();

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
	m_VertexArray->AddVertexBuffer(VB);

	uint32_t indices[3 * 2] = {
		0, 1, 2,
		0, 2, 3
	};
	Ref<IndexBuffer> IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

	m_VertexArray->SetIndexBuffer(IB);

	m_FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Timestep ts)
{
	// ---- Update ----
	m_CameraController.OnUpdate(ts);

	// ---- Render ----
	RenderCommand::SetClearColor({ ColorFormat::RGBADecimal, 25 });
	RenderCommand::Clear();

	Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	m_FlatColorShader->Bind();

	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
			if ((x % 2 == 0 && y % 2 == 0) || (x % 2 != 0 && y % 2 != 0))
			{
				std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_Color1);
			}
			else {
				std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_Color2);
			}
			Renderer::Submit(m_FlatColorShader, m_VertexArray, transform);
		}
	}

	Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("2D Settings");

	ImGui::ColorEdit3("Color 1", glm::value_ptr(m_Color1));
	ImGui::ColorEdit3("Color 2", glm::value_ptr(m_Color2));

	ImGui::End();
}

// ---- Event Handling ----

void Sandbox2D::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}
