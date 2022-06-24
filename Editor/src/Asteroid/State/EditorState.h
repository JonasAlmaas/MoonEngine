#pragma once

using namespace Moon;


namespace Asteroid {

	class EditorState
	{
	public:
		static void Init();

		static Ref<Framebuffer> GetFramebuffer();

		static void SetActiveScene(Ref<Scene>& scene);
		static Ref<Scene> GetActiveScene();

		static Entity GetSelectionContext();
		static void SetSelectionContext(Entity entity = {});

		static entt::registry* GetSceneRegistry();

	};

}
