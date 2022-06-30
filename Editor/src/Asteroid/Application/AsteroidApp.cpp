#include "aopch.h"
#include "Asteroid/Application/AsteroidApp.h"

#include "Asteroid/Layer/EditorLayer.h"


namespace Asteroid {

	AsteroidApp::AsteroidApp(ApplicationCommandLineArgs args)
		: Application("Asteroid", args)
	{
		PushLayer(CreateRef<EditorLayer>());
	}

}
