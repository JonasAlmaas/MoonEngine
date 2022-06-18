#include "Application/AsteroidApp.h"

#include "EditorLayer/EditorLayer.h"


namespace Asteroid {

	AsteroidApp::AsteroidApp()
	{
		PushLayer(CreateRef<EditorLayer>());
	}

}
