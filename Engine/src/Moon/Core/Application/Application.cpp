#include "mepch.h"
#include "Moon/Core/Application/Application.h"

#include "Moon/Core/Renderer/Buffer/VertexBuffer/VertexBufferLayout.h"

// TEMPORARY
#include <glad/glad.h>


namespace Moon {

	Application* Application::s_Instance = nullptr;

	// TEMPORARY
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Moon::ShaderDataType::Bool:		return GL_BOOL;
			case Moon::ShaderDataType::Int:			return GL_INT;
			case Moon::ShaderDataType::Int2:		return GL_INT;
			case Moon::ShaderDataType::Int3:		return GL_INT;
			case Moon::ShaderDataType::Int4:		return GL_INT;
			case Moon::ShaderDataType::Float:		return GL_FLOAT;
			case Moon::ShaderDataType::Float2:		return GL_FLOAT;
			case Moon::ShaderDataType::Float3:		return GL_FLOAT;
			case Moon::ShaderDataType::Float4:		return GL_FLOAT;
			case Moon::ShaderDataType::Mat3:		return GL_FLOAT;
			case Moon::ShaderDataType::Mat4:		return GL_FLOAT;
		}
		
		ME_CORE_ASSERT(false, "Unknows ShaderDataType!");
		return 0;
	}
	
	Application::Application()
	{
		ME_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(ME_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// TEOPORARY
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[7 * 3] = {
		//   x      y     z     r     g     b     a
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			VertexBufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
			};
			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		uint32_t indices[] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

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
			// TODO: Remove glad once this is gone!
			glClearColor(0.1, 0.1, 0.1, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
			// ----

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
