#include "aopch.h"
#include "Asteroid/Panel/UILibrary/UILibrary.h"


namespace Asteroid {

	bool UILibrary::DrawFloatControl(const std::string& label, float& value, float resetValue, float speed, float min, float max, const char* format, bool isLast, float firstColumnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, firstColumnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		if (isLast)
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 6 });
		else
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 2 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight, lineHeight };

		float sliderWidth = ImGui::GetContentRegionAvail().x - buttonSize.x - 3.0f;

		ImGui::SetNextItemWidth(sliderWidth);
		bool hasChanged = ImGui::DragFloat("##X", &value, speed, min, max, format);

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

	bool UILibrary::DrawFloat2Control(const std::string& label, const std::string& xLabel, const std::string& yLabel, glm::vec2& values, float speed, float resetValue, bool isLast, float firstColumnWidth)
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
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 6 });
		else
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 2 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight, lineHeight };

		float sliderWidth = ImGui::GetContentRegionAvail().x / 2.0f - buttonSize.x - 1.0f;

		// -- X --
		{
			if (isLast)
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 6 });
			else
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 2 });

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f, 0.0f });

			ImGui::PushFont(boldFont);

			if (ImGui::Button(xLabel.c_str(), buttonSize))
			{
				values.x = resetValue;
				changed = true;
			}

			ImGui::PopFont();
			ImGui::PopStyleVar();

			ImGui::SameLine();
			ImGui::SetNextItemWidth(sliderWidth);
			if (ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f"))
				changed = true;

			ImGui::PopStyleVar();
		}

		// -- Y --
		{
			ImGui::SameLine();

			if (isLast)
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 6 });
			else
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 2 });

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f, 0.0f });

			ImGui::PushFont(boldFont);

			if (ImGui::Button(yLabel.c_str(), buttonSize))
			{
				values.y = resetValue;
				changed = true;
			}

			ImGui::PopFont();
			ImGui::PopStyleVar();

			ImGui::SameLine();
			ImGui::SetNextItemWidth(sliderWidth);
			if (ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f"))
				changed = true;

			ImGui::PopStyleVar();
		}

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
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
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 6 });
		else
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 2 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight, lineHeight };

		float sliderWidth = ImGui::GetContentRegionAvail().x / 3.0f - buttonSize.x - 2.0f;

		// -- X --
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

			if (isLast)
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 6 });
			else
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 2 });

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f, 0.0f });

			ImGui::PushFont(boldFont);

			if (ImGui::Button("X", buttonSize))
			{
				values.x = resetValue;
				changed = true;
			}

			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(sliderWidth);
			if (ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f"))
				changed = true;

			ImGui::PopStyleVar();
		}

		// -- Y --
		{
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

			if (isLast)
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 6 });
			else
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 2 });

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f, 0.0f });

			ImGui::PushFont(boldFont);

			if (ImGui::Button("Y", buttonSize))
			{
				values.y = resetValue;
				changed = true;
			}

			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(sliderWidth);
			if (ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f"))
				changed = true;

			ImGui::PopStyleVar();
		}

		// -- Z --
		{
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

			if (isLast)
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 6 });
			else
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 2 });

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f, 0.0f });
			ImGui::PushFont(boldFont);

			if (ImGui::Button("Z", buttonSize))
			{
				values.z = resetValue;
				changed = true;
			}

			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(sliderWidth);
			if (ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f"))
				changed = true;

			ImGui::PopStyleVar();
		}

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
	}

	bool UILibrary::DrawColor3Control(const std::string& label, float* value, bool isLast, float firstColumnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, firstColumnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		if (isLast)
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 9 });
		else
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 5 });

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x + 3.0f);
		bool changed = ImGui::ColorEdit3("##Color", value);

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return changed;
	}

	bool UILibrary::DrawColor4Control(const std::string& label, float* value, bool isLast, float firstColumnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, firstColumnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		if (isLast)
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 9 });
		else
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 5 });

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x + 3.0f);
		bool changed = ImGui::ColorEdit4("##Color", value);

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return changed;
	}

	bool UILibrary::Checkbox(const std::string& label, bool* value, bool isLast, float firstColumnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, firstColumnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		if (isLast)
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 9 });
		else
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 5 });

		bool changed = ImGui::Checkbox("##Checkbox", value);

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return changed;
	}

}
