#include "aopch.h"
#include "Asteroid/Panel/Properties/PropertiesPanel.h"

#include "Asteroid/State/EditorState.h"


namespace Asteroid {

	static bool DrawFloatControl(const std::string& label, float &value, float speed = 0.1f, float resetValue = 0.0f, bool isLast = false, float firstColumnWidth = 125.0f)
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

	static void DrawFloat3Control(const std::string& label, glm::vec3& values, float speed = 0.1f, float resetValue = 0.0f, bool isLast = false, float firstColumnWidth = 125.0f)
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
			ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
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
			ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
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
			ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f");
		}

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, bool allowRemove, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::Separator();
			
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (allowRemove)
			{
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}

				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove component"))
						removeComponent = true;

					ImGui::EndPopup();
				}
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

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

			// -- Transform Component --
			DrawComponent<TransformComponent>("Transform", selectionContext, true, [](TransformComponent& component)
			{
				DrawFloat3Control("Translation", component.Translation, 0.01f);
				
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawFloat3Control("Rotation", rotation, 0.01f);
				component.Rotation = glm::radians(rotation);

				DrawFloat3Control("Scale", component.Scale, 0.01f, 1.0f, true);
			});

			// -- Camera Component --
			DrawComponent<CameraComponent>("Camera", selectionContext, false, [](CameraComponent& component)
			{
				auto& camera = component.Camera;

				ImGui::PushID("Projection");

				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text("Projection");
				ImGui::NextColumn();

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 4 });

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo("##Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::PopStyleVar();

				ImGui::Columns(1);

				ImGui::PopID();

				switch (camera.GetProjectionType())
				{
					case SceneCamera::ProjectionType::Perspective:
					{
						float perspecVerticalFOV = camera.GetPerspectiveVerticalFOV();
						if (DrawFloatControl("Vertical FOV", perspecVerticalFOV, 0.5f, 45.0f))
							camera.SetPerspectiveVerticalFOV(perspecVerticalFOV);

						float perspecNearClip = camera.GetPerspectiveNearClip();
						if (DrawFloatControl("Near Clip", perspecNearClip, 0.1f, 0.01f))
							camera.SetPerspectiveNearClip(perspecNearClip);

						float perspecFarClip = camera.GetPerspectiveFarClip();
						if (DrawFloatControl("Far Clip", perspecFarClip, 1.0f, 10000.0f, true))
							camera.SetPerspectiveFarClip(perspecFarClip);

						break;
					}
					case SceneCamera::ProjectionType::Orthographic:
					{
						float orthoSize = camera.GetOrthographicSize();
						if (DrawFloatControl("Size", orthoSize, 0.1f, 10.0f))
							camera.SetOrthographicSize(orthoSize);

						float orthoNearClip = camera.GetOrthographicNearClip();
						if (DrawFloatControl("Near Clip", orthoNearClip, 0.01f, -1.0f))
							camera.SetOrthographicNearClip(orthoNearClip);

						float orthoFarClip = camera.GetOrthographicFarClip();
						if (DrawFloatControl("Far Clip", orthoFarClip, 0.01f, 1.0f, true))
							camera.SetOrthographicFarClip(orthoFarClip);

						break;
					}
					default:
					{
						ME_CORE_ASSERT(false, "Unknown ProjectionType");
						break;
					}
				}

			});

			// -- Sprite Renderer Component --
			if (selectionContext.HasComponent<SpriteRendererComponent>())
			{

			}
		}

		ImGui::End();
	}

}
