#include "aopch.h"
#include "Asteroid/Panel/Properties/PropertiesPanel.h"

#include "Asteroid/Panel/UILibrary/UILibrary.h"
#include "Asteroid/State/EditorState.h"


namespace Asteroid {

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
				UILibrary::DrawFloat3Control("Translation", component.Translation, 0.01f);
				
				glm::vec3 rotation = glm::degrees(component.Rotation);
				UILibrary::DrawFloat3Control("Rotation", rotation, 0.01f);
				component.Rotation = glm::radians(rotation);

				UILibrary::DrawFloat3Control("Scale", component.Scale, 0.01f, 1.0f, true);
			});

			// -- Camera Component --
			DrawComponent<CameraComponent>("Camera", selectionContext, false, [](CameraComponent& component)
			{
				auto& camera = component.Camera;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				UILibrary::DrawCombo("Projection", projectionTypeStrings, 2, (int)camera.GetProjectionType(), component, [](CameraComponent& component, uint32_t i)
				{
					component.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
				});

				switch (camera.GetProjectionType())
				{
					case SceneCamera::ProjectionType::Perspective:
					{
						float perspecVerticalFOV = camera.GetPerspectiveVerticalFOV();
						if (UILibrary::DrawFloatControl("Vertical FOV", perspecVerticalFOV, 0.5f, 45.0f))
							camera.SetPerspectiveVerticalFOV(perspecVerticalFOV);

						float perspecNearClip = camera.GetPerspectiveNearClip();
						if (UILibrary::DrawFloatControl("Near Clip", perspecNearClip, 0.1f, 0.01f))
							camera.SetPerspectiveNearClip(perspecNearClip);

						float perspecFarClip = camera.GetPerspectiveFarClip();
						if (UILibrary::DrawFloatControl("Far Clip", perspecFarClip, 1.0f, 10000.0f))
							camera.SetPerspectiveFarClip(perspecFarClip);

						break;
					}
					case SceneCamera::ProjectionType::Orthographic:
					{
						float orthoSize = camera.GetOrthographicSize();
						if (UILibrary::DrawFloatControl("Size", orthoSize, 0.1f, 10.0f))
							camera.SetOrthographicSize(orthoSize);

						float orthoNearClip = camera.GetOrthographicNearClip();
						if (UILibrary::DrawFloatControl("Near Clip", orthoNearClip, 0.01f, -1.0f))
							camera.SetOrthographicNearClip(orthoNearClip);

						float orthoFarClip = camera.GetOrthographicFarClip();
						if (UILibrary::DrawFloatControl("Far Clip", orthoFarClip, 0.01f, 1.0f))
							camera.SetOrthographicFarClip(orthoFarClip);

						break;
					}
					default:
					{
						ME_CORE_ASSERT(false, "Unknown ProjectionType");
						break;
					}
				}

				UILibrary::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio, true);

			});

			// -- Sprite Renderer Component --
			DrawComponent<SpriteRendererComponent>("Sprite Renderer", selectionContext, false, [](SpriteRendererComponent& component)
			{
				UILibrary::DrawColor4Control("Color", component.Color);
			});
		}

		ImGui::End();
	}

}
