#pragma once

using namespace Moon;

#include "Asteroid/Scene/EditorScene.h"


namespace Asteroid {

	class EditorState
	{
	public:
		static void Init();

		static Ref<Framebuffer> GetFramebuffer();

		static Ref<EditorScene> NewActiveScene();
		static void SetActiveScene(Ref<EditorScene>& scene);
		static Ref<EditorScene> GetActiveScene();

	};

}
