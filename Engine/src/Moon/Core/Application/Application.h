#pragma once

#include "Moon/Core/Base.h"

#include "Moon/Core/Event/ApplicationEvent.h"

#include "Moon/Core/Layer/LayerStack.h"

#include "Moon/Core/Renderer/IndexBuffer/IndexBuffer.h"
#include "Moon/Core/Renderer/Shader/Shader.h"
#include "Moon/Core/Renderer/VertexBuffer/VertexBuffer.h"
#include "Moon/Core/Renderer/VertexArray/VertexArray.h"

#include "Moon/Core/Window/Window.h"

#include "Moon/ImGui/Layer/ImGuiLayer.h"


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

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<VertexArray> m_TriangleVA;

	private:
		static Application* s_Instance;

	};

	// To be defined in client
	Application* CreateApplication();

}
