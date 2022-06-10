#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Timestep.h"
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

		/**
		 * @return delta time in the Timestep format.
		 */
		Timestep GetTimestep() const { return m_Timestep; }
		
		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClosed(WindowCloseEvent& e);

	private:
		bool m_Running = true;

		std::unique_ptr<Window> m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;
		Timestep m_Timestep = 0.0f;

	private:
		static Application* s_Instance;

	};

	// To be defined in client
	Application* CreateApplication();

}
