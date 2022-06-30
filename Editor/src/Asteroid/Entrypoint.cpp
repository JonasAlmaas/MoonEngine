#include "aopch.h"
#include <Moon/Core/Entrypoint.h>

#include "Asteroid/Application/AsteroidApp.h"


namespace Moon {

	Application* Application::Create(ApplicationCommandLineArgs args)
	{
		return new Asteroid::AsteroidApp(args);
	}

}
