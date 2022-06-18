#include "Editor/Application/AsteroidApp.h"

#include "Editor/Layer/Editor/EditorLayer.h"


namespace Asteroid {

	AsteroidApp::AsteroidApp()
		: Application("Asteroid")
	{
		PushLayer(CreateRef<EditorLayer>());
	}

}
