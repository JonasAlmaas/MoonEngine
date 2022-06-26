#pragma once

using namespace Moon;


namespace Asteroid {

	class EditorState
	{
	public:
		static void Init();

		static Ref<Framebuffer> GetFramebuffer();

		static Ref<Scene> NewActiveScene();
		static void SetActiveScene(Ref<Scene>& scene);
		static Ref<Scene> GetActiveScene();

	};

}
