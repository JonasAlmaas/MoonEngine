#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Event/ApplicationEvent.h"
#include "Moon/Core/Layer/LayerStack.h"
#include "Moon/Core/Renderer/Buffer/VertexBuffer/VertexBuffer.h"
#include "Moon/Core/Renderer/Buffer/IndexBuffer/IndexBuffer.h"
#include "Moon/Core/Window/Window.h"
#include "Moon/ImGui/Layer/ImGuiLayer.h"

// TEMPORARY
#include "Moon/Core/Renderer/Shader/Shader.h"


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
		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

	private:
		static Application* s_Instance;

	};

	// To be defined in client
	Application* CreateApplication();

}
