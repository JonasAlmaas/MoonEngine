#include <Moon.h>
#include <Moon/Core/Entrypoint.h>

#include "Moon/Platform/OpenGL/Shader/OpenGLShader.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Moon {

	class ExampleLayer : public Layer
	{
	public:
		ExampleLayer()
			: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
		{
			// ---- Square ----
			{
				m_SquareVA.reset(VertexArray::Create());

				float vertices[3 * 4] = {
					-0.5f, -0.5f, 0.0f,
					 0.5f, -0.5f, 0.0f,
					 0.5f,  0.5f, 0.0f,
					-0.5f,  0.5f, 0.0f,
				};
				std::shared_ptr<VertexBuffer> VB;
				VB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

				VertexBufferLayout layout = {
					{ ShaderDataType::Float3, "Position XYZ" },
				};
				VB->SetLayout(layout);
				m_SquareVA->AddVertexBuffer(VB);

				uint32_t indices[3 * 2] = {
					0, 1, 2,
					0, 2, 3
				};
				std::shared_ptr<IndexBuffer> IB;
				IB.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

				m_SquareVA->SetIndexBuffer(IB);
			}

			// ---- Triangle ----
			{
				m_TriangleVA.reset(VertexArray::Create());

				float vertices[3 * 3] = {
						-0.5f, -0.5f, 0.0f,
						 0.5f, -0.5f, 0.0f,
						 0.0f,  0.5f, 0.0f,
				};
				std::shared_ptr<VertexBuffer> VB;
				VB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

				VertexBufferLayout layout = {
					{ ShaderDataType::Float3, "Position XYZ" },
				};
				VB->SetLayout(layout);
				m_TriangleVA->AddVertexBuffer(VB);

				uint32_t indices[3] = {
					0, 1, 2
				};
				std::shared_ptr<IndexBuffer> IB;
				IB.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

				m_TriangleVA->SetIndexBuffer(IB);
			}

			// ---- Basic Temp Shader ----
			std::string vertexSrc = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec3 v_Position;

				void main()
				{
					v_Position = a_Position;

					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
				}
			)";

			std::string fragmentSrc = R"(
				#version 330 core

				layout(location = 0) out vec4 o_Color;

				in vec3 v_Position;
				in vec4 v_Color;

				uniform vec3 u_Color;

				void main()
				{
					o_Color = vec4(u_Color, 1.0);
				}
			)";

			m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));
		}

		virtual void OnImGuiRender() override
		{
			ImGui::Begin("Settings");

			ImGui::ColorEdit3("Color 1", glm::value_ptr(m_Color1));
			ImGui::ColorEdit3("Color 2", glm::value_ptr(m_Color2));

			ImGui::End();
		}

		void OnUpdate(Timestep ts) override
		{
			if (Input::IsMouseButtonPressed(Mouse::Button2))
			{
				auto [x, y] = Input::GetMousePosition();

				m_CameraPosition.x += (m_LastMousePosition.x - x) * 0.0016666666666667f;
				m_CameraPosition.y -= (m_LastMousePosition.y - y) * 0.00140625f;

				m_LastMousePosition = { x, y };

				m_Camera.SetPosition(m_CameraPosition);
			}

			RenderCommand::SetClearColor({ ColorFormat::RGBADecimal, 25 });
			RenderCommand::Clear();

			Renderer::BeginScene(m_Camera);

			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

			m_Shader->Bind();

			for (int x = 0; x < 20; x++)
			{
				for (int y = 0; y < 20; y++)
				{
					glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
					if ((x % 2 == 0 && y % 2 == 0) || (x % 2 != 0 && y % 2 != 0))
					{
						std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_Color1);
					}
					else {
						std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_Color2);
					}
					Renderer::Submit(m_Shader, m_SquareVA, transform);
				}
			}

			std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", { 0.8f, 0.2f, 0.3f });
			Renderer::Submit(m_Shader, m_TriangleVA);

			Renderer::EndScene();
		}

		// ---- Event handling ----

		void OnEvent(Event& e) override
		{
			EventDispatcher dispatcher(e);

			dispatcher.Dispatch<MouseButtonPressedEvent>(ME_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressedEvent));
		}

		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
		{
			// Middle mouse
			if (e.GetMouseButton() == Mouse::Button2)
			{
				auto [x, y] = Input::GetMousePosition();
				m_LastMousePosition = { x, y };
			}

			return false;
		}

	private:
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<VertexArray> m_TriangleVA;

		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_LastMousePosition = { 0.0f, 0.0f };

		glm::vec3 m_Color1 = { 0.8f, 0.2f, 0.3f };
		glm::vec3 m_Color2 = { 0.3f, 0.2f, 0.8f};

	};

	class Sandbox : public Application
	{
	public:
		Sandbox()
		{
			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
			io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f);

			PushLayer(new ExampleLayer());
		}

		~Sandbox()
		{

		}

	};
	
	Application* CreateApplication()
	{
		return new Sandbox();
	}

}
