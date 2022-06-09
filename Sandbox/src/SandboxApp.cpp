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
