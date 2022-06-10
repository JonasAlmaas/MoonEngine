#include "mepch.h"
#include "Moon/Core/Application/Application.h"

#include "Moon/Core/Renderer/Renderer.h"

// TEMPORARY
#include "Moon/Core/Renderer/VertexBuffer/VertexBufferLayout.h"


namespace Moon {

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		ME_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(ME_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		
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

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;

				gl_Position = vec4(a_Position, 1.0);
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

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor({ ColorFormat::RGBADecimal, 25 });
			RenderCommand::Clear();

			Renderer::BeginScene();
			{
				m_Shader->Bind();
				Renderer::Submit(m_SquareVA);

				Renderer::Submit(m_TriangleVA);
			}
			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	// ---- Event handling ----

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(ME_BIND_EVENT_FN(Application::OnWindowClosed));

		// Send events to layers
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}
