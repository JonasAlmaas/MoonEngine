#pragma once

using namespace Moon;

#include <Moon/Scene/Scene.h>


namespace Asteroid {

	class EditorScene : public Scene
	{
	public:
		virtual ~EditorScene() = default;

		/**
		 * Create a copy of the scene.
		 * To be used in runtime.
		 */
		Ref<EditorScene> Copy();

		void OnUpdateEditor(Timestep ts, const Ref<RenderCamera>& camera);

		Entity GetSelectionContext() { return m_SelectionContext; }
		void SetSelectionContext(Entity entity = {}) { m_SelectionContext = entity; }

	private:
		Entity m_SelectionContext{};

	};

}
