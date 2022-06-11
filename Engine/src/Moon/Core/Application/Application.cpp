#include "mepch.h"
#include "Moon/Core/Application/Application.h"

#include "Moon/Core/Renderer/Renderer.h"

// TODO: TEMPORARY
#include <GLFW/glfw3.h>


namespace Moon {

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		ME_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(ME_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = CreateRef<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// Get delta time
			float time = (float)glfwGetTime();	// Platfrom::GetTime();
			m_Timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Ref<Layer> layer : m_LayerStack)
				layer->OnUpdate(m_Timestep);

			m_ImGuiLayer->Begin();
			for (Ref<Layer> layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Ref<Layer> layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Ref<Layer> overlay)
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
