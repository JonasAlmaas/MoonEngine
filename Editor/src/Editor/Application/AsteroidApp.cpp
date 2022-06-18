#include "Editor/Application/AsteroidApp.h"

#include "Editor/Layer/Editor/EditorLayer.h"


namespace Asteroid {

	AsteroidApp::AsteroidApp()
	{
		PushLayer(CreateRef<EditorLayer>());
	}

}
