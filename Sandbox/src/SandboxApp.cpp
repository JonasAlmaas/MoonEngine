#include <Moon.h>
#include <Moon/Core/Entrypoint.h>

#include <imgui.h>


class ExampleLayer : public Moon::Layer
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

	void OnEvent(Moon::Event& e) override
	{

	}

};

class Sandbox : public Moon::Application
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

Moon::Application* Moon::CreateApplication()
{
	return new Sandbox();
}
