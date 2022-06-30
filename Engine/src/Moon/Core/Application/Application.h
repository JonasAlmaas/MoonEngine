#pragma once

#include "Moon/Core/Layer/LayerStack.h"
#include "Moon/Core/Window/Window.h"
#include "Moon/ImGui/ImGuiLayer.h"


namespace Moon {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			ME_CORE_ASSERT(index < Count, "Index out of range!");
			return Args[index];
		}
	};
	
	class Application
	{
	public:
		Application(const std::string& name = "Moon Application", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		// To be defined in client
		static Application* Create(ApplicationCommandLineArgs args);
		
		/**
		 * @brief Starts the main application loop.
		 */
		void Run();

		/**
		 * @brief Closes Application.
		 */
		void Close()
		{
			m_Running = false;
		}

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

		inline Ref<ImGuiLayer> GetImGuiLayer() { return m_ImGuiLayer; }

		/**
		 * @return Delta time in the Timestep format.
		 */
		inline Timestep GetTimestep() const { return m_Timestep; }
		
		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

	private:
		bool OnWindowClosedEvent(WindowCloseEvent& e);
		bool OnWindowMinimizeEvent(WindowMinimizeEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		ApplicationCommandLineArgs m_CommandLineArgs;

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
