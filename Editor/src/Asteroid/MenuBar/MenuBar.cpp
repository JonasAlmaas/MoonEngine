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
				if (ImGui::MenuItem("Serialize"))
				{
					SceneSerializer serializer(EditorState::GetActiveScene());
					serializer.Serialize("Content/Scenes/LVL_Test.masset");
				}

				if (ImGui::MenuItem("Deserialize"))
				{
					SceneSerializer serializer(EditorState::GetActiveScene());
					serializer.Deserialize("Content/Scenes/LVL_Test.masset");
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt+F4"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

}
