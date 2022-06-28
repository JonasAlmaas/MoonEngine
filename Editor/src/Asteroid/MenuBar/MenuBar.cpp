#include "aopch.h"
#include "Asteroid/MenuBar/MenuBar.h"

#include "Asteroid/State/EditorState.h"

#include <Moon/Scene/Serializer/SceneSerializer.h>


namespace Asteroid {

	void MenuBar::OnImGuiRender()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt+F4"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	// -- Event Handling --

	void MenuBar::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressedEvent>(ME_BIND_EVENT_FN(MenuBar::OnKeyPressed));
	}

	bool MenuBar::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool ctrlPressed = (Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl));
		bool shiftPressed = (Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift));

		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (ctrlPressed)
					NewScene();
				break;
			}
			case Key::O:
			{
				if (ctrlPressed)
					OpenScene();
				break;
			}
			case Key::S:
			{
				if (ctrlPressed)
				{
					if (shiftPressed)
						SaveSceneAs();
					else
						SaveScene();
				}
				break;
			}
			default:
				break;
		}

		return false;
	}

	// -- Shortcuts --

	void MenuBar::NewScene()
	{
		EditorState::NewActiveScene();
	}

	void MenuBar::SaveSceneAs()
	{
		std::string filepath = FileDialog::SaveFile("Moon Scene (*.mmap)\0*.mmap\0");

		if (!filepath.empty())
		{
			SceneSerializer serializer(EditorState::GetActiveScene());
			serializer.Serialize(filepath);
		}
	}

	void MenuBar::SaveScene()
	{
	}

	void MenuBar::OpenScene()
	{
		std::string filepath = FileDialog::OpenFile("Moon Scene (*.mmap)\0*.mmap\0");

		if (!filepath.empty())
		{
			Ref<Scene> activeScene = EditorState::NewActiveScene();
			SceneSerializer serializer(activeScene);
			serializer.Deserialize(filepath);
		}
	}

}
