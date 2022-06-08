#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Events/ApplicationEvent.h"
#include "Moon/Core/Layer/LayerStack.h"
#include "Moon/Core/Window/Window.h"
#include "Moon/ImGui/Layer/ImGuiLayer.h"

// TEMPORARY
#include "Moon/Renderer/Shader/Shader.h"


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

		// TEOPORARY
		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

	private:
		static Application* s_Instance;

	};

	// To be defined in client
	Application* CreateApplication();

}
