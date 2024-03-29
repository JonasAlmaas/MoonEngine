#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Application.h"


#ifdef ME_PLATFORM_WINDOWS

	int main(int argc, char** argv)
	{
		Moon::Log::Init();

		ME_PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
		auto app = Moon::Application::Create({ argc, argv });
		ME_PROFILE_END_SESSION();

		ME_PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
		app->Run();
		ME_PROFILE_END_SESSION();

		ME_PROFILE_BEGIN_SESSION("Shutdown", "Profile-Shutdown.json");
		delete app;
		ME_PROFILE_END_SESSION();

		return 0;
	}

#endif
