#include "mepch.h"
#include "Moon/Core/Application/Application.h"

#include "Moon/Core/Renderer/Renderer.h"
#include "Moon/Core/Util/Util.h"

// TODO: TEMP!
#include "Moon/Core/Util/Random/Random.h"


namespace Moon {

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		ME_PROFILE_FUNCTION();

		ME_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(ME_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		Util::Init();

		m_ImGuiLayer = CreateRef<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		ME_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::Run()
	{
		ME_PROFILE_FUNCTION();

		while (m_Running)
		{	
			ME_PROFILE_SCOPE("RunLoop");

			// Get delta time
			float time = Util::GetTime();
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

	void Application::PushLayer(Ref<Layer> layer)
	{
		ME_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Ref<Layer> overlay)
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
