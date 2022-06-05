#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Application/Application.h"


#ifdef ME_PLATFORM_WINDOWS
	
	extern Moon::Application* Moon::CreateApplication();
	
	int main(int argc, char** argv)
	{
		auto app = Moon::CreateApplication();
		app->Run();
		delete app;
	}

#endif
