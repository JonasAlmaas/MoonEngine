#include "Application/SandboxApp.h"

#include "Sandbox2D/Sandbox2D.h"


namespace Sandbox {

	SandboxApp::SandboxApp()
	{
		PushLayer(CreateRef<Sandbox2D>());
	}

}
