#include "aopch.h"
#include <Moon/Core/Entrypoint.h>

#include "Asteroid/Application/AsteroidApp.h"


namespace Moon {

	Application* Application::Create(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Asteroid";
		spec.WorkingDirectory = "../Editor";
		spec.CommandLineArgs = args;

		return new Asteroid::AsteroidApp(spec);
	}

}
