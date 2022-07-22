#include "mepch.h"
#include "Moon/Core/Application/Application.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Core/Util/Util.h"


namespace Moon {

	Application* Application::s_Instance = nullptr;
	
	Application::Application(const ApplicationSpecification& specs)
		: m_Specification(specs)
	{
		ME_PROFILE_FUNCTION();

		ME_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = Window::Create({ m_Specification.Name });
		m_Window->SetEventCallback(ME_BIND_EVENT_FN(Application::OnEvent));

		Util::Init();
		Renderer::Init();

		if (specs.EnableImGui)
		{
			m_ImGuiLayer = new ImGuiLayer();
			PushOverlay(m_ImGuiLayer);
		}
	}

	Application::~Application()
	{
		ME_PROFILE_FUNCTION();

		for (Layer* layer : m_LayerStack)
		{
			layer->OnDetach();
			delete layer;
		}

		Renderer::Shutdown();
	}

	void Application::Run()
	{
		ME_PROFILE_FUNCTION();

		while (m_Running)
		{	
			ME_PROFILE_SCOPE("RunLoop");

			// Get delta time
			float time = Time::Get();
			m_Timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				m_LayerStack.OnUpdate(m_Timestep);
			}

			m_ImGuiLayer->Begin();
			m_LayerStack.OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		ME_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		ME_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	// ---- Event Handling ----

	void Application::OnEvent(Event& e)
	{
		ME_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(ME_BIND_EVENT_FN(Application::OnWindowClosedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(ME_BIND_EVENT_FN(Application::OnWindowResizeEvent));
		dispatcher.Dispatch<WindowMinimizeEvent>(ME_BIND_EVENT_FN(Application::OnWindowMinimizeEvent));

		m_LayerStack.OnEvent(e);
	}

	bool Application::OnWindowClosedEvent(WindowCloseEvent& e)
	{
		ME_PROFILE_FUNCTION();

		m_Running = false;
		return true;
	}

	bool Application::OnWindowMinimizeEvent(WindowMinimizeEvent& e)
	{
		ME_PROFILE_FUNCTION();

		m_Minimized = e.GetMinimizedState();
		return false;
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ME_PROFILE_FUNCTION();

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

}
