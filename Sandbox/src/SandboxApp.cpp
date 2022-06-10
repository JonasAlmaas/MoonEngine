#include <Moon.h>
#include <Moon/Core/Entrypoint.h>

#include <imgui.h>


namespace Moon {

	class ExampleLayer : public Layer
	{
	public:
		ExampleLayer()
			: Layer("Example")
		{
		}

		virtual void OnImGuiRender() override
		{

		}

		void OnUpdate() override
		{

		}

		void OnEvent(Event& e) override
		{

		}

	};

	class Sandbox : public Application
	{
	public:
		Sandbox()
		{
			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
			io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f);

			PushLayer(new ExampleLayer());
		}

		~Sandbox()
		{

		}

	};
	
	Application* CreateApplication()
	{
		return new Sandbox();
	}

}
