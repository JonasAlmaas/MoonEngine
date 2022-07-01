#include "aopch.h"
#include "Asteroid/State/EditorState.h"

#include <Moon/Scene/Serializer/SceneSerializer.h>


namespace Asteroid {

	struct EditorStateData
	{
		Ref<Framebuffer> Framebuffer;
		Ref<EditorScene> ActiveScene;
		Ref<EditorCamera> EditorCamera;

		Ref<EditorState::PanelState> PanelState;
	};

	static EditorStateData s_Data;

	void EditorState::Init()
	{
		FramebufferSpecification spec;
		spec.Attachments = {
			FramebufferTextureFormat::RGBA8,		// Color
			FramebufferTextureFormat::RED_INTEGER,	// Entity ID
			FramebufferTextureFormat::Depth			// Depth
		};
		s_Data.Framebuffer = Framebuffer::Create(spec);

		s_Data.EditorCamera = CreateRef<EditorCamera>();
		s_Data.PanelState = CreateRef<PanelState>();

		NewActiveScene();

		// Load scene if a path was specified as a command line argument
		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(s_Data.ActiveScene);
			serializer.Deserialize(sceneFilePath);
		}
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

	void EditorState::NewScene()
	{
		NewActiveScene();
	}

	void EditorState::SaveSceneAs()
	{
		std::string filepath = FileDialog::SaveFile("Moon Scene (*.mmap)\0*.mmap\0");

		if (!filepath.empty())
		{
			SceneSerializer serializer(EditorState::GetActiveScene());
			serializer.Serialize(filepath);
		}
	}

	void EditorState::SaveScene()
	{
	}

	void EditorState::OpenScene()
	{
		std::string filepath = FileDialog::OpenFile("Moon Scene (*.mmap)\0*.mmap\0");
		if (!filepath.empty())
			OpenScene(filepath);
	}

	void EditorState::OpenScene(const std::filesystem::path& path)
	{
		Ref<Scene> activeScene = EditorState::NewActiveScene();
		SceneSerializer serializer(activeScene);
		serializer.Deserialize(path.string());
	}

	Ref<EditorCamera> EditorState::GetEditorCamera()
	{
		return s_Data.EditorCamera;
	}

	Ref<EditorState::PanelState> EditorState::GetPanelState()
	{
		return s_Data.PanelState;
	}

}
