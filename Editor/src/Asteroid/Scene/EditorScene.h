#pragma once

using namespace Moon;

#include <Moon/Scene/Scene.h>


namespace Asteroid {

	class EditorScene : public Scene
	{
	public:
		virtual ~EditorScene() = default;

		Entity GetSelectionContext() { return m_SelectionContext; }
		void SetSelectionContext(Entity entity = {}) { m_SelectionContext = entity; }

	private:
		Entity m_SelectionContext{};

	};

}
