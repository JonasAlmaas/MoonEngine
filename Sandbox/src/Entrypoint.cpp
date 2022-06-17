#include <Moon/Core/Entrypoint.h>

#include "Application/SandboxApp.h"


namespace Moon {

	Application* Application::Create()
	{
		return new Sandbox::SandboxApp();
	}

}
