#pragma once

#include "Moon/Core/Base.h"
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

	private:
		bool OnWindowClosed(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in client
	Application* CreateApplication();

}
