#include "aopch.h"
#include <Moon/Core/Entrypoint.h>

#include "Asteroid/Core/Application.h"


namespace Moon {

	Application* Application::Create(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.CommandLineArgs = args;
		spec.Name = "Asteroid";
		spec.WorkingDirectory = "../Moon-Editor";
		spec.WindowWidth = 1920;
		spec.WindowHeight = 1080;
		spec.StartMaximized = true;
		spec.VSync = true;

		return new Asteroid::AsteroidApp(spec);
	}

}
