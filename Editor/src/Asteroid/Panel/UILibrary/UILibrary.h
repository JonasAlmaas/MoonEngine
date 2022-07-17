#pragma once

using namespace Moon;


namespace Asteroid {

	class UILibrary
	{
	public:
		static float s_FirstColumnWidth;

		static bool DrawFloatControl(const std::string& label, float& value, float resetValue = 0.0f, float speed = 0.1f, float min = 0.0f, float max = 0.0f, const char* format = "%.2f", bool isLast = false);
		static bool DrawFloat2Control(const std::string& label, const std::string& xLabel, const std::string& yLabel, glm::vec2& values, float resetValue = 0.0f, float speed = 0.1f, bool isLast = false);
		static bool DrawFloat3Control(const std::string& label, glm::vec3& values, float speed = 0.1f, float resetValue = 0.0f, bool isLast = false);

		static bool DrawColor3Control(const std::string& label, float* value, bool isLast = false);
		static bool DrawColor4Control(const std::string& label, float* value, bool isLast = false);

		static bool Checkbox(const std::string& label, bool* value, bool isLast = false);

		template<typename UIFunction>
		static void DrawCombo(const std::string& label, const char* options[], uint32_t optionsCount, uint32_t activeIndex, auto& component, UIFunction uiFunction, bool isLast = false)
		{
			ImGui::PushID(label.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, s_FirstColumnWidth);
			ImGui::Text("Projection");
			ImGui::NextColumn();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 5 });

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x + 2);
			if (ImGui::BeginCombo("##Combo", options[activeIndex]))
			{
				for (uint32_t i = 0; i < optionsCount; i++)
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
