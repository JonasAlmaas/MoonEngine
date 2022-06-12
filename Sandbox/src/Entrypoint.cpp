#include <Moon/Core/Entrypoint.h>

#include "Application/SandboxApp.h"


namespace Moon {

	Application* CreateApplication()
	{
		return new SandboxApp();
	}

}
