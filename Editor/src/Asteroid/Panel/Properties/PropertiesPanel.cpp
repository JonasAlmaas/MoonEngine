#include "aopch.h"
#include "Asteroid/Panel/Properties/PropertiesPanel.h"

#include "Asteroid/State/EditorState.h"


namespace Asteroid {

	void PropertiesPanel::OnAttach()
	{
	}

	void PropertiesPanel::OnDetach()
	{
	}

	void PropertiesPanel::OnUpdate(Timestep ts)
	{
	}

	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::Begin("Properties");

		Entity selectionContext = EditorState::GetSelectionContext();

		if (selectionContext)
		{
			// -- Tag Component --
			if (selectionContext.HasComponent<TagComponent>())
			{
				auto& tag = selectionContext.GetComponent<TagComponent>().Tag;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tag.c_str());

				ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;

				ImGui::PushItemWidth(-1);
				if (ImGui::InputText("##", buffer, sizeof(buffer), flags))
				{
					tag = std::string(buffer);
				}
				ImGui::PopItemWidth();
			}
		}

		ImGui::End();
	}

}
