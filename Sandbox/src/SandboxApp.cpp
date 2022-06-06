#include <Moon.h>
#include <Moon/Core/Application/Entrypoint.h>


class ExampleLayer : public Moon::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{

	}

	void OnEvent(Moon::Event& e) override
	{
		ME_TRACE("{0}", e);
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
