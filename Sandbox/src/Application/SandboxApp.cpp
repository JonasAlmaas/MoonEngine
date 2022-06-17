#include "Application/SandboxApp.h"

#include "Sandbox2D/Sandbox2D.h"



SandboxApp::SandboxApp()
{
	//PushLayer(CreateRef<ExampleLayer>());
	PushLayer(CreateRef<Sandbox2D>());
}
