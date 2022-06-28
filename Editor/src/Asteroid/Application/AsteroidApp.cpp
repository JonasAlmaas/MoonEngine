#include "aopch.h"
#include "Asteroid/Application/AsteroidApp.h"

#include "Asteroid/Layer/EditorLayer.h"


namespace Asteroid {

	AsteroidApp::AsteroidApp()
		: Application("Asteroid")
	{
		PushLayer(CreateRef<EditorLayer>());
	}

}
