#include <Moon.h>
#include <Moon/Core/Entrypoint.h>

#include <imgui.h>


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

				float vertices[7 * 4] = {
					//   x      y     z     r     g     b     a
						-0.75f, -0.75f, 0.0f, 0.8f, 0.2f, 0.3f, 1.0f,
						 0.75f, -0.75f, 0.0f, 0.8f, 0.2f, 0.3f, 1.0f,
						 0.75f,  0.75f, 0.0f, 0.8f, 0.2f, 0.3f, 1.0f,
						-0.75f,  0.75f, 0.0f, 0.8f, 0.2f, 0.3f, 1.0f,
				};
				std::shared_ptr<VertexBuffer> VB;
				VB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

				VertexBufferLayout layout = {
					{ ShaderDataType::Float3, "Position XYZ" },
					{ ShaderDataType::Float4, "Color RGBA"},
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

				float vertices[7 * 3] = {
					//   x      y     z     r     g     b     a
						-0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
						 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
						 0.0f,  0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
				};
				std::shared_ptr<VertexBuffer> VB;
				VB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

				VertexBufferLayout layout = {
					{ ShaderDataType::Float3, "Position XYZ" },
					{ ShaderDataType::Float4, "Color RGBA"},
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
				layout(location = 1) in vec4 a_Color;

				uniform mat4 u_ViewProjection;

				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;

					gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
				}
			)";

			std::string fragmentSrc = R"(
				#version 330 core

				layout(location = 0) out vec4 o_Color;

				in vec3 v_Position;
				in vec4 v_Color;

				void main()
				{
					o_Color = v_Color;
				}
			)";

			m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		}

		virtual void OnImGuiRender() override
		{

		}

		void OnUpdate(Timestep ts) override
		{
			ME_TRACE("Delta time {0}s, {1}ms", ts.GetSeconds(), ts.GetMilliseconds());

			if (Input::IsKeyPressed(Key::W))
			{
				m_CameraPosition.x -= m_CameraMoveSpeed * ts * sin(glm::radians(m_CameraRotation));
				m_CameraPosition.y += m_CameraMoveSpeed * ts * cos(glm::radians(m_CameraRotation));
			}

			if (Input::IsKeyPressed(Key::A))
			{
				m_CameraPosition.x -= m_CameraMoveSpeed * ts * cos(glm::radians(m_CameraRotation));
				m_CameraPosition.y -= m_CameraMoveSpeed * ts * sin(glm::radians(m_CameraRotation));
			}

			if (Input::IsKeyPressed(Key::S))
			{
				m_CameraPosition.x += m_CameraMoveSpeed * ts * sin(glm::radians(m_CameraRotation));
				m_CameraPosition.y -= m_CameraMoveSpeed * ts * cos(glm::radians(m_CameraRotation));
			}

			if (Input::IsKeyPressed(Key::D))
			{
				m_CameraPosition.x += m_CameraMoveSpeed * ts * cos(glm::radians(m_CameraRotation));
				m_CameraPosition.y += m_CameraMoveSpeed * ts * sin(glm::radians(m_CameraRotation));
			}

			if (Input::IsKeyPressed(Key::Q))
			{
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}

			if (Input::IsKeyPressed(Key::E))
			{
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}

			m_Camera.SetPosition(m_CameraPosition);
			m_Camera.SetRotationDegrees(m_CameraRotation);

			RenderCommand::SetClearColor({ ColorFormat::RGBADecimal, 25 });
			RenderCommand::Clear();

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_Shader, m_SquareVA);
			Renderer::Submit(m_Shader, m_TriangleVA);
			
			Renderer::EndScene();
		}

		// ---- Event handling ----

		void OnEvent(Event& e) override
		{

		}

	private:
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<VertexArray> m_TriangleVA;

		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 40.0f;
		float m_CameraMoveSpeed = 1.0f;

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
