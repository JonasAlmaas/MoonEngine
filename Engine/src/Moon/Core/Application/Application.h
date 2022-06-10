#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Event/ApplicationEvent.h"
#include "Moon/Core/Layer/LayerStack.h"
#include "Moon/Core/Window/Window.h"
#include "Moon/ImGui/ImGuiLayer.h"


namespace Moon {
	
	class Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void OnEvent(Event& e);
		
		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClosed(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;

	private:
		static Application* s_Instance;

	};

	// To be defined in client
	Application* CreateApplication();

}
