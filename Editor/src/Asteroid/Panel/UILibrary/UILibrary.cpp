#include "aopch.h"
#include "Asteroid/Panel/UILibrary/UILibrary.h"


namespace Asteroid {

	bool UILibrary::DrawFloatControl(const std::string& label, float& value, float speed, float resetValue, bool isLast, float firstColumnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, firstColumnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		if (isLast)
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 9 });
		else
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 4 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight, lineHeight };

		float sliderWidth = ImGui::GetContentRegionAvail().x - buttonSize.x - 4;

		ImGui::SetNextItemWidth(sliderWidth);
		bool hasChanged = ImGui::DragFloat("##X", &value, speed, 0.0f, 0.0f, "%.2f");

		ImGui::SameLine();
		ImGui::PushFont(boldFont);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f, 0.0f });
		if (ImGui::Button("<", buttonSize))
		{
			value = resetValue;
			hasChanged = true;
		}
		ImGui::PopStyleVar();
		ImGui::PopFont();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return hasChanged;
	}

	bool UILibrary::DrawFloat3Control(const std::string& label, glm::vec3& values, float speed, float resetValue, bool isLast, float firstColumnWidth)
	{
		bool changed = false;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, firstColumnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		if (isLast)
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 9 });
		else
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 4 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight, lineHeight };

		float sliderWidth = ImGui::GetContentRegionAvail().x / 3.0f - buttonSize.x - 6.0f;

		// -- X --
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

			ImGui::PushFont(boldFont);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f, 0.0f });
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopStyleVar();
			ImGui::PopFont();

			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(sliderWidth);
			if (ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f"))
				changed = true;
		}

		// -- Y --
		{
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

			ImGui::PushFont(boldFont);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f, 0.0f });
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopStyleVar();
			ImGui::PopFont();

			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(sliderWidth);
			if (ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f"))
				changed = true;
		}

		// -- Z --
		{
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

			ImGui::PushFont(boldFont);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f, 0.0f });
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopStyleVar();
			ImGui::PopFont();

			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(sliderWidth);
			if (ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f"))
				changed = true;
		}

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
	}
	}

}
