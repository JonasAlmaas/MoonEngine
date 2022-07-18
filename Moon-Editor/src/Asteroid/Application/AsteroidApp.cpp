#include "aopch.h"
#include "Asteroid/Application/AsteroidApp.h"

#include "Asteroid/Layer/EditorLayer.h"


namespace Asteroid {

	AsteroidApp::AsteroidApp(const ApplicationSpecification& specs)
		: Application(specs)
	{
		PushLayer(CreateRef<EditorLayer>());
	}

}
