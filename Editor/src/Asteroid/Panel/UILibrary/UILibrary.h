#pragma once

using namespace Moon;


namespace Asteroid {

	class UILibrary
	{
	public:
		static bool DrawFloatControl(const std::string& label, float& value, float speed = 0.1f, float resetValue = 0.0f, bool isLast = false, float firstColumnWidth = 175.0f);
		static bool DrawFloat3Control(const std::string& label, glm::vec3& values, float speed = 0.1f, float resetValue = 0.0f, bool isLast = false, float firstColumnWidth = 175.0f);

		template<typename UIFunction>
		static void DrawCombo(const std::string& label, const char* options[], uint32_t optionsCount, uint32_t activeIndex, auto& component, UIFunction uiFunction, bool isLast = false, float firstColumnWidth = 125.0f)
		{
			ImGui::PushID(label.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 125.0f);
			ImGui::Text("Projection");
			ImGui::NextColumn();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 4 });

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::BeginCombo("##Combo", options[activeIndex]))
			{
				for (int i = 0; i < optionsCount; i++)
				{
					bool isSelected = options[activeIndex] == options[i];
					if (ImGui::Selectable(options[i], isSelected))
						uiFunction(component, i);

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::PopStyleVar();

			ImGui::Columns(1);

			ImGui::PopID();
		}

	};

}
