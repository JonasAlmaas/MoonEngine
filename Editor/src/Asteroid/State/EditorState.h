#pragma once

using namespace Moon;

#include "Asteroid/Scene/EditorScene.h"


namespace Asteroid {

	class EditorState
	{
	public:
		struct PanelState
		{
			bool ContentBrowser = true;
			bool Properties = true;
			bool Renderer = true;
			bool SceneHierarchy = true;
			bool Viewport = true;
		};

		static void Init();

		static Ref<Framebuffer> GetFramebuffer();

		static Ref<EditorScene> NewActiveScene();
		static void SetActiveScene(Ref<EditorScene>& scene);
		static Ref<EditorScene> GetActiveScene();

		static Ref<EditorCamera> GetEditorCamera();

		static Ref<PanelState> GetPanelState();

	};

}
