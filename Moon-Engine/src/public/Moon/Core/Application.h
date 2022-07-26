#pragma once

#include "Moon/Core/Event/WindowEvent.h"
#include "Moon/Core/LayerStack.h"
#include "Moon/Core/Window.h"
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

	struct ApplicationSpecification
	{
		std::string Name = "Moon Application";
		std::string WorkingDirectory;
		uint32_t WindowWidth = 1920, WindowHeight = 1080;
		bool WindowDecorated = true;
		bool Fullscreen = false;
		bool VSync = true;
		bool StartMaximized = false;
		bool Resizable = true;
		bool EnableImGui = true;
		ApplicationCommandLineArgs CommandLineArgs;
	};
	
	class Application
	{
	public:
		Application(const ApplicationSpecification& spec);
		virtual ~Application();

		// To be defined in client
		static Application* Create(ApplicationCommandLineArgs args);
		
		void Run();
		void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void OnEvent(Event& e);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		Timestep GetTimestep() const { return m_Timestep; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

	private:
		bool OnWindowClosedEvent(WindowCloseEvent& e);
		bool OnWindowMinimizeEvent(WindowMinimizeEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		ApplicationSpecification m_Specification;
		Scope<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;

	};

}
