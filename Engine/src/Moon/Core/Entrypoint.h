#pragma once

#include "Moon/Core/Application/Application.h"


#ifdef ME_PLATFORM_WINDOWS

	int main(int argc, char** argv)
	{
		Moon::Log::Init();

		auto app = Moon::Application::Create();
		app->Run();
		delete app;

		return 0;
	}

#endif
