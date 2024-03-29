#include "aopch.h"
#include "Asteroid/EditorState.h"

#include <Moon/Scene/SceneSerializer.h>


namespace Asteroid {

	struct EditorStateData
	{
		Ref<Framebuffer> Framebuffer;

		Ref<EditorScene> EditorSn;
		Ref<EditorScene> RuntimeSn;
		std::filesystem::path EditorScenePath;

		Ref<EditorCamera> EditorCamera;

		Ref<PanelState> PanelState;
		SceneState SceneState = SceneState::Edit;

		bool ShowPhysicsColliders = false;

		EditorState::TextureLibrary TextureLibrary;
	};

	static EditorStateData* s_Data = nullptr;

	static void GenerateTextures()
	{
		// Generate checkerboard texture
		{
			ME_PROFILE_SCOPE("GenerateTexture-Checkerboard");

			uint32_t* textureData = new uint32_t[2 * 2]{ 0xffcccccc, 0xffffffff, 0xffffffff, 0xffcccccc };
			s_Data->TextureLibrary.Checkerboard = Texture2D::Create(ImageFormat::RGBA8,2, 2);
			s_Data->TextureLibrary.Checkerboard->SetData(textureData, sizeof(uint32_t) * 2 * 2);
			delete[] textureData;
		}

		// Generate color grid texture
		{
			ME_PROFILE_SCOPE("GenerateTexture-ColorGrid");

			uint32_t width = 16;
			uint32_t height = 16;
			uint32_t* textureData = new uint32_t[width * height];
			for (uint8_t x = 0; x < width; x++)
			{
				for (uint8_t y = 0; y < height; y++)
				{
					uint8_t r = (uint8_t)((1.0f - (float)(x + 1) / (float)width) * 255.0f);

					uint8_t g = 0;
					float g1 = (float)(x - y) / (float)(width + height - 2) * 255.0f;
					if (g1 > 0.0f)
						g = (uint8_t)g1;

					uint8_t b = (uint8_t)(((float)(y + 1) / (float)height) * 255.0f);

					textureData[x + y * width] = r + (g << 8) + (b << 16) | 0xff000000;
				}
			}
			s_Data->TextureLibrary.ColorGrid = Texture2D::Create(ImageFormat::RGBA8, width, height);
			s_Data->TextureLibrary.ColorGrid->SetData(textureData, sizeof(uint32_t) * width * height);
			delete[] textureData;
		}
	}

	void EditorState::Init()
	{
		s_Data = new EditorStateData();

		FramebufferSpecification spec;
		spec.Attachments = {
			FramebufferTextureFormat::RGBA8,		// Color
			FramebufferTextureFormat::RED_INTEGER,	// Entity ID
			FramebufferTextureFormat::Depth			// Depth
		};
		s_Data->Framebuffer = Framebuffer::Create(spec);

		s_Data->EditorCamera = CreateRef<EditorCamera>();
		s_Data->PanelState = CreateRef<PanelState>();

		NewActiveScene();

		// Load scene if a path was specified as a command line argument
		auto appSpec = Application::Get().GetSpecification();
		if (appSpec.CommandLineArgs.Count > 1)
		{
			s_Data->EditorScenePath = std::filesystem::path(appSpec.CommandLineArgs[1]);
			SceneSerializer serializer(s_Data->EditorSn);
			serializer.Deserialize(s_Data->EditorScenePath);
		}

		GenerateTextures();
	}

	void EditorState::Shutdown()
	{
		delete s_Data;
		s_Data = nullptr;
	}

	Ref<Framebuffer> EditorState::GetFramebuffer()
	{
		return s_Data->Framebuffer;
	}

	// ---- Scene ----

	Ref<EditorScene> EditorState::NewActiveScene()
	{
		s_Data->EditorSn = CreateRef<EditorScene>();

		auto& spec = s_Data->Framebuffer->GetSpecification();
		s_Data->EditorSn->OnViewportResize(spec.Width, spec.Height);

		return s_Data->EditorSn;
	}

	void EditorState::SetActiveScene(Ref<EditorScene>& scene)
	{
		s_Data->EditorSn = scene;
	}

	Ref<EditorScene> EditorState::GetActiveScene()
	{
		switch (s_Data->SceneState)
		{
			case SceneState::Edit:		return s_Data->EditorSn;
			case SceneState::Play:		return s_Data->RuntimeSn;
			case SceneState::Simulate:	return s_Data->RuntimeSn;
		}

		ME_CORE_ASSERT(false, "Unknown scene state!");
		return s_Data->EditorSn;
	}

	void EditorState::NewScene()
	{
		NewActiveScene();
		s_Data->EditorScenePath = std::filesystem::path();
	}

	void EditorState::SaveSceneAs()
	{
		std::string filepath = FileDialog::SaveFile("Moon Scene (*.mmap)\0*.mmap\0");

		if (!filepath.empty())
		{
			s_Data->EditorScenePath = std::filesystem::path(filepath);

			SceneSerializer serializer(EditorState::GetActiveScene());
			serializer.Serialize(filepath);
		}
	}

	void EditorState::SaveScene()
	{
		if (!s_Data->EditorScenePath.empty())
		{
			SceneSerializer serializer(EditorState::GetActiveScene());
			serializer.Serialize(s_Data->EditorScenePath);
		}
		else
		{
			SaveSceneAs();
		}
	}

	void EditorState::OpenScene()
	{
		std::string filepath = FileDialog::OpenFile("Moon Scene (*.mmap)\0*.mmap\0");
		if (!filepath.empty())
		{
			s_Data->EditorScenePath = std::filesystem::path(filepath);
			OpenScene(filepath);
		}
	}

	void EditorState::OpenScene(const std::filesystem::path& path)
	{
		if (s_Data->SceneState != SceneState::Edit)
			OnSceneStop();

		if (path.extension().string() != ".mmap")
		{
			ME_CORE_LOG_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		Ref<Scene> activeScene = EditorState::NewActiveScene();
		SceneSerializer serializer(activeScene);
		serializer.Deserialize(path.string());
	}

	void EditorState::OnScenePlay()
	{
		s_Data->SceneState = SceneState::Play;
		s_Data->RuntimeSn = s_Data->EditorSn->Copy();
		s_Data->RuntimeSn->OnRuntimeStart();
	}

	void EditorState::OnSceneStop()
	{
		s_Data->SceneState = SceneState::Edit;
		s_Data->RuntimeSn->OnRuntimeStop();
		s_Data->RuntimeSn = nullptr;
	}

	void EditorState::OnSimulateStart()
	{
		s_Data->SceneState = SceneState::Simulate;
		s_Data->RuntimeSn = s_Data->EditorSn->Copy();
		s_Data->RuntimeSn->OnSimulationStart();
	}

	void EditorState::OnSimulateStop()
	{
		s_Data->SceneState = SceneState::Edit;
		s_Data->RuntimeSn->OnSimulationStop();
		s_Data->RuntimeSn = nullptr;
	}

	SceneState EditorState::GetSceneState()
	{
		return s_Data->SceneState;
	}

	// ---- ----

	bool EditorState::GetShowPhysicsColliders()
	{
		return s_Data->ShowPhysicsColliders;
	}

	void EditorState::SetShowPhysicsColliders(bool state)
	{
		s_Data->ShowPhysicsColliders = state;
	}

	const EditorState::TextureLibrary& EditorState::GetTextureLibrary()
	{
		return s_Data->TextureLibrary;
	}

	Ref<EditorCamera> EditorState::GetEditorCamera()
	{
		return s_Data->EditorCamera;
	}

	Ref<PanelState> EditorState::GetPanelState()
	{
		return s_Data->PanelState;
	}

}
