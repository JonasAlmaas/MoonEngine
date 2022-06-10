#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Event/ApplicationEvent.h"
#include "Moon/Core/ImGui/ImGuiLayer.h"
#include "Moon/Core/Layer/LayerStack.h"
#include "Moon/Core/Window/Window.h"


namespace Moon {
	
	class Application
	{
	public:
		Application();
		virtual ~Application();
		
		/**
		 * @brief Starts the main application loop.
		 */
		void Run();

		/**
		 * Adds a layer to the layer stack.
		 * 
		 * @param A child class of Layer. Initialized with the keyword new.
		 */
		void PushLayer(Layer* layer);

		/**
		 * Adds an overlay to the layer stack.
		 *
		 * @param A child class of Layer. Initialized with the keyword new.
		 */
		void PushOverlay(Layer* overlay);

		/**
		 * Handler for all application and input events.
		 * Distributes events to the rest of the application.
		 * 
		 * @param A child class of Event.
		 */
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
