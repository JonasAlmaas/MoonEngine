#include "mepch.h"
#include "Moon/Core/Application.h"

#include "Moon/Core/Renderer.h"
#include "Moon/Core/Util.h"
#include "Moon/Scripting/ScriptEngine.h"


namespace Moon {

	Application* Application::s_Instance = nullptr;
	
	Application::Application(const ApplicationSpecification& spec)
		: m_Specification(spec)
	{
		ME_PROFILE_FUNCTION();

		ME_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		{
			WindowSpecification windowSpec;
			windowSpec.Title = spec.Name;
			windowSpec.Width = spec.WindowWidth;
			windowSpec.Height = spec.WindowHeight;
			windowSpec.Decorated = spec.WindowDecorated;
			windowSpec.Fullscreen = spec.Fullscreen;
			windowSpec.VSync = spec.VSync;

			m_Window = Window::Create(windowSpec);
			m_Window->Init();

			m_Window->SetEventCallback(ME_BIND_EVENT_FN(Application::OnEvent));

			if (spec.StartMaximized)
				m_Window->Maximize();
			else
				m_Window->CenterWindow();

			m_Window->SetResizable(spec.Resizable);
		}

		Util::Init();
		Renderer::Init();
		ScriptEngine::Init();

		if (spec.EnableImGui)
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

		ScriptEngine::Shutdown();
		Renderer::Shutdown();
	}

	void Application::Run()
	{
		ME_PROFILE_FUNCTION();

		while (m_Running)
		{	
			ME_PROFILE_SCOPE("RunLoop");

			static uint64_t frameCounter = 0;

			// Get delta time
			float time = Time::Get();
			m_Timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				m_LayerStack.OnUpdate(m_Timestep);
			}

			if (m_Specification.EnableImGui)
			{
				m_ImGuiLayer->Begin();
				m_LayerStack.OnImGuiRender();
				m_ImGuiLayer->End();
			}

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

		Close();
		return true;
	}

	bool Application::OnWindowMinimizeEvent(WindowMinimizeEvent& e)
	{
		ME_PROFILE_FUNCTION();

		m_Minimized = e.IsMinimized();
		return false;
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ME_PROFILE_FUNCTION();

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

}
