#include "ExampleLayer/ExampleLayer.h"

#include <Moon/Platform/OpenGL/Shader/OpenGLShader.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


ExampleLayer::ExampleLayer()
	: Layer("ExampleLayer"), m_CameraController((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), true, true, true, true)
{

}

void ExampleLayer::OnAttach()
{
	// ---- Square ----
	{
		m_SquareVA = VertexArray::Create();

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
		m_SquareVA->AddVertexBuffer(VB);

		uint32_t indices[3 * 2] = {
			0, 1, 2,
			0, 2, 3
		};
		Ref<IndexBuffer> IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_SquareVA->SetIndexBuffer(IB);
	}

	// ---- Triangle ----
	{
		m_TriangleVA = VertexArray::Create();

		float vertices[3 * 3] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f,
		};
		Ref<VertexBuffer> VB = VertexBuffer::Create(vertices, sizeof(vertices));

		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "Position XYZ" },
		};
		VB->SetLayout(layout);
		m_TriangleVA->AddVertexBuffer(VB);

		uint32_t indices[3] = {
			0, 1, 2
		};
		Ref<IndexBuffer> IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_TriangleVA->SetIndexBuffer(IB);
	}

	m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
	m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

	m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
	m_TransparentTexture = Texture2D::Create("assets/textures/TransparencyTest.png");

	Ref<Shader> textureShader = m_ShaderLibrary.Get("Texture");
	textureShader->Bind();
	std::dynamic_pointer_cast<OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Timestep ts)
{
	// ---- Update ----
	m_CameraController.OnUpdate(ts);

	// ---- Render ----

	RenderCommand::SetClearColor({ ColorFormat::RGBADecimal, 25 });
	RenderCommand::Clear();

	Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	Ref<Shader> flatColorShader = m_ShaderLibrary.Get("FlatColor");
	flatColorShader->Bind();

	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
			if ((x % 2 == 0 && y % 2 == 0) || (x % 2 != 0 && y % 2 != 0))
			{
				std::dynamic_pointer_cast<OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_Color1);
			}
			else {
				std::dynamic_pointer_cast<OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_Color2);
			}
			Renderer::Submit(flatColorShader, m_SquareVA, transform);
		}
	}

	Ref<Shader> textureShader = m_ShaderLibrary.Get("Texture");

	m_Texture->Bind();
	Renderer::Submit(textureShader, m_SquareVA);

	m_TransparentTexture->Bind();
	Renderer::Submit(textureShader, m_SquareVA);

	std::dynamic_pointer_cast<OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", { 0.8f, 0.2f, 0.3f });
	Renderer::Submit(flatColorShader, m_TriangleVA);

	Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Example Settings");

	ImGui::ColorEdit3("Color 1", glm::value_ptr(m_Color1));
	ImGui::ColorEdit3("Color 2", glm::value_ptr(m_Color2));

	ImGui::End();
}

// ---- Event Handling ----

void ExampleLayer::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}
