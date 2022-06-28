#include "aopch.h"
#include "Asteroid/State/EditorState.h"


namespace Asteroid {

	struct EditorStateData
	{
		Ref<Framebuffer> Framebuffer;
		Ref<EditorScene> ActiveScene;
		Ref<EditorCamera> EditorCamera;
	};

	static EditorStateData s_Data;

	void EditorState::Init()
	{
		FramebufferSpecification spec;
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		s_Data.Framebuffer = Framebuffer::Create(spec);

		s_Data.EditorCamera = CreateRef<EditorCamera>();

		NewActiveScene();
	}

	Ref<Framebuffer> EditorState::GetFramebuffer()
	{
		return s_Data.Framebuffer;
	}

	Ref<EditorScene> EditorState::NewActiveScene()
	{
		s_Data.ActiveScene = CreateRef<EditorScene>();

		auto& spec = s_Data.Framebuffer->GetSpecification();
		s_Data.ActiveScene->OnViewportResize(spec.Width, spec.Height);

		return s_Data.ActiveScene;
	}

	void EditorState::SetActiveScene(Ref<EditorScene>& scene)
	{
		s_Data.ActiveScene = scene;
	}

	Ref<EditorScene> EditorState::GetActiveScene()
	{
		return s_Data.ActiveScene;
	}

	Ref<EditorCamera> EditorState::GetEditorCamera()
	{
		return s_Data.EditorCamera;
	}

}
