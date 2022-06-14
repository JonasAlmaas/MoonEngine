#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Event/ApplicationEvent.h"
#include "Moon/Core/Layer/LayerStack.h"
#include "Moon/Core/Type/Timestep.h"
#include "Moon/Core/Window/Window.h"
#include "Moon/ImGui/ImGuiLayer.h"


namespace Moon {
	
	class Application
	{
	public:
		Application();
		virtual ~Application();

		// To be defined in client
		static Application* Create();
		
		/**
		 * @brief Starts the main application loop.
		 */
		void Run();

		/**
		 * Adds a layer to the layer stack.
		 * 
		 * @param A child class of Layer. Initialized with the keyword new.
		 */
		void PushLayer(Ref<Layer> layer);

		/**
		 * Adds an overlay to the layer stack.
		 *
		 * @param A child class of Layer. Initialized with the keyword new.
		 */
		void PushOverlay(Ref<Layer> overlay);

		/**
		 * Handler for all application and input events.
		 * Distributes events to the rest of the application.
		 * 
		 * @param A child class of Event.
		 */
		void OnEvent(Event& e);

		/**
		 * @return Delta time in the Timestep format.
		 */
		inline Timestep GetTimestep() const { return m_Timestep; }
		
		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClosedEvent(WindowCloseEvent& e);
		bool OnWindowMinimizeEvent(WindowMinimizeEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		bool m_Running = true;
		bool m_Minimized = false;

		Scope<Window> m_Window;

		LayerStack m_LayerStack;
		Ref<ImGuiLayer> m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;
		Timestep m_Timestep = 0.0f;

	private:
		static Application* s_Instance;

	};

}
