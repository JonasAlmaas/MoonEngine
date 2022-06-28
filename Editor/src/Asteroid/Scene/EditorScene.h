#pragma once

using namespace Moon;

#include <Moon/Scene/Scene.h>

#include "Asteroid/Camera/EditorCamera.h"


namespace Asteroid {

	class EditorScene : public Scene
	{
	public:
		virtual ~EditorScene() = default;

		void OnUpdateEditor(Timestep ts, const Ref<EditorCamera>& camera);

		Entity GetSelectionContext() { return m_SelectionContext; }
		void SetSelectionContext(Entity entity = {}) { m_SelectionContext = entity; }

	private:
		Entity m_SelectionContext{};

	};

}
