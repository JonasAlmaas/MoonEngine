#include "Application/SandboxApp.h"

#include "ExampleLayer/ExampleLayer.h"
#include "Sandbox2D/Sandbox2D.h"



SandboxApp::SandboxApp()
{
	//PushLayer(CreateRef<ExampleLayer>());
	PushLayer(CreateRef<Sandbox2D>());
}
