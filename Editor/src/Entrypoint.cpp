#include <Moon.h>
#include <Moon/Core/Entrypoint.h>

#include "Application/AsteroidApp.h"


namespace Moon {

	Application* Application::Create()
	{
		return new Asteroid::AsteroidApp();
	}

}
