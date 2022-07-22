#include "aopch.h"
#include "Asteroid/Application/AsteroidApp.h"

#include "Asteroid/Layer/EditorLayer.h"


namespace Asteroid {

	AsteroidApp::AsteroidApp(const ApplicationSpecification& spec)
		: Application(spec)
	{
		PushLayer(new EditorLayer());
	}

}
