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
					EditorState::NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					EditorState::OpenScene();

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					EditorState::SaveScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					EditorState::SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt+F4"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				auto panelState = EditorState::GetPanelState();

				if (ImGui::MenuItem("ContentBrowser", NULL, panelState->ContentBrowser))
					panelState->ContentBrowser = !panelState->ContentBrowser;

				if (ImGui::MenuItem("Properties", NULL, panelState->Properties))
					panelState->Properties = !panelState->Properties;

				if (ImGui::MenuItem("Renderer", NULL, panelState->Renderer))
					panelState->Renderer = !panelState->Renderer;

				if (ImGui::MenuItem("SceneHierarchy", NULL, panelState->SceneHierarchy))
					panelState->SceneHierarchy = !panelState->SceneHierarchy;

				if (ImGui::MenuItem("Viewport", NULL, panelState->Viewport))
					panelState->Viewport = !panelState->Viewport;

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
					EditorState::NewScene();
				break;
			}
			case Key::O:
			{
				if (ctrlPressed)
					EditorState::OpenScene();
				break;
			}
			case Key::S:
			{
				if (ctrlPressed)
				{
					if (shiftPressed)
						EditorState::SaveSceneAs();
					else
						EditorState::SaveScene();
				}
				break;
			}
			default:
				break;
		}

		return false;
	}

}
