#include "aopch.h"
#include "Asteroid/Core/Application.h"

#include "Asteroid/EditorLayer.h"


namespace Asteroid {

	AsteroidApp::AsteroidApp(const ApplicationSpecification& spec)
		: Application(spec)
	{
		PushLayer(new EditorLayer());
	}

}
