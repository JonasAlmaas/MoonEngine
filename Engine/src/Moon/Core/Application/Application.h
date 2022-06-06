#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Layer/LayerStack.h"
#include "Moon/Core/Window/Window.h"
#include "Moon/Events/ApplicationEvent.h"


namespace Moon {
	
	class MOON_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		
		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClosed(WindowCloseEvent& e);

	private:
		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;

		bool m_Running = true;
	};

	// To be defined in client
	Application* CreateApplication();

}
