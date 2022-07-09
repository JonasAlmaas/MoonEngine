#pragma once

using namespace Moon;

#include "Asteroid/Scene/EditorScene.h"


namespace Asteroid {

	enum class SceneState
	{
		Edit = 0,
		Play,
		Pause,
		Simulate,
	};

	struct PanelState
	{
		bool ContentBrowser = true;
		bool Properties = true;
		bool Renderer = true;
		bool SceneHierarchy = true;
		bool Settings = true;
		bool Toolbar = true;
		bool Viewport = true;
	};

	class EditorState
	{
	public:
		struct TextureLibrary
		{
			Ref<Texture2D> Checkerboard;
			Ref<Texture2D> ColorGrid;
		};

	public:
		static void Init();

		static Ref<Framebuffer> GetFramebuffer();

		// ---- Scene ----
		static Ref<EditorScene> NewActiveScene();

		static void SetActiveScene(Ref<EditorScene>& scene);
		static Ref<EditorScene> GetActiveScene();

		static void NewScene();
		static void SaveSceneAs();
		static void SaveScene();
		static void OpenScene();
		static void OpenScene(const std::filesystem::path& path);

		static void OnScenePlay();
		static void OnSceneStop();

		static void OnSimulateStart();
		static void OnSimulateStop();

		static SceneState GetSceneState();

		static bool GetShowPhysicsColliders();
		static void SetShowPhysicsColliders(bool state);

		static const TextureLibrary& GetTextureLibrary();

		static Ref<EditorCamera> GetEditorCamera();

		static Ref<PanelState> GetPanelState();

	};

}
