#include "aopch.h"
#include "Asteroid/State/EditorState.h"


namespace Asteroid {

	struct EditorStateData
	{
		Ref<Framebuffer> Framebuffer;
		Ref<Scene> ActiveScene;
		Entity SelectionContext;
	};

	static EditorStateData s_Data;

	void EditorState::Init()
	{
		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		s_Data.Framebuffer = Framebuffer::Create(spec);
	}

	Ref<Framebuffer> EditorState::GetFramebuffer()
	{
		return s_Data.Framebuffer;
	}

	void EditorState::SetActiveScene(Ref<Scene>& scene)
	{
		s_Data.ActiveScene = scene;
	}

	Ref<Scene> EditorState::GetActiveScene()
	{
		return s_Data.ActiveScene;
	}

	Entity EditorState::GetSelectionContext()
	{
		return s_Data.SelectionContext;
	}

	void EditorState::SetSelectionContext(Entity entity)
	{
		s_Data.SelectionContext = entity;
	}

	entt::registry* EditorState::GetSceneRegistry()
	{
		return &s_Data.ActiveScene->GetRegistry();
	}

}
