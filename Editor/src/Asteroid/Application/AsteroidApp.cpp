#include "aopch.h"
#include "Asteroid/Application/AsteroidApp.h"

#include "Asteroid/Layer/Editor/EditorLayer.h"


namespace Asteroid {

	AsteroidApp::AsteroidApp()
		: Application("Asteroid")
	{
		PushLayer(CreateRef<EditorLayer>());
	}

}
