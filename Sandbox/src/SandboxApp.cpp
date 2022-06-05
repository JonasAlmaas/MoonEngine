#include <Moon.h>


class Sandbox : public Moon::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

Moon::Application* Moon::CreateApplication()
{
	return new Sandbox();
}
