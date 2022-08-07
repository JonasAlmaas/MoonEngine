#include "aopch.h"
#include "Asteroid/Panel/PropertiesPanel.h"

#include "Asteroid/Panel/UILibrary.h"
#include "Asteroid/EditorState.h"


namespace Asteroid {

	extern const std::filesystem::path g_ContentPath;

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
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

			if (open)
			{
				uiFunction(entity, component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	template<typename T>
	static void DisplayAddComponentEntry(const std::string& lable, Entity selectionContext)
	{
		if (!selectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(lable.c_str()))
			{
				selectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
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

		Entity selectionContext = EditorState::GetActiveScene()->GetSelectionContext();

		if (selectionContext)
		{
			// -- Tag Component -- and add component
			if (selectionContext.HasComponent<TagComponent>())
			{
				ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

				auto& tag = selectionContext.GetComponent<TagComponent>().Tag;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tag.c_str());

				ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5.0f, 4.0f });

				ImGui::PushItemWidth(contentRegionAvailable.x - lineHeight - 3);
				if (ImGui::InputText("##", buffer, sizeof(buffer), flags))
					tag = std::string(buffer);
				ImGui::PopItemWidth();

				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
				if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
					ImGui::OpenPopup("AddComponent");

				ImGui::PopStyleVar(2);

				if (ImGui::BeginPopup("AddComponent"))
				{
					DisplayAddComponentEntry<TransformComponent>("Add Transform Component", selectionContext);
					DisplayAddComponentEntry<CameraComponent>("Add Camera Component", selectionContext);
					DisplayAddComponentEntry<SpriteRendererComponent>("Add Sprite Renderer Component", selectionContext);
					DisplayAddComponentEntry<CircleRendererComponent>("Add Circle Renderer Component", selectionContext);
					DisplayAddComponentEntry<Rigidbody2DComponent>("Add Rigid Body 2D Component", selectionContext);
					DisplayAddComponentEntry<BoxCollider2DComponent>("Add Box Collider 2D Component", selectionContext);
					DisplayAddComponentEntry<CircleCollider2DComponent>("Add Circle Collider 2D Component", selectionContext);

					ImGui::EndPopup();
				}
			}

			// -- Transform Component --
			DrawComponent<TransformComponent>("Transform", selectionContext, [](Entity& entity, TransformComponent& component)
			{
				UILibrary::DrawFloat3Control("Translation", component.Translation, 0.01f);
				
				glm::vec3 rotation = glm::degrees(component.Rotation);
				UILibrary::DrawFloat3Control("Rotation", rotation, 0.01f);
				component.Rotation = glm::radians(rotation);

				UILibrary::DrawFloat3Control("Scale", component.Scale, 0.01f, 1.0f, true);
			});

			// -- Camera Component --
			DrawComponent<CameraComponent>("Camera", selectionContext, [](Entity& entity, CameraComponent& component)
			{
				auto& camera = component.Camera;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				UILibrary::DrawCombo("Projection", projectionTypeStrings, 2, (int)camera->GetProjectionType(), component, [](CameraComponent& component, uint32_t i)
				{
					component.Camera->SetProjectionType((SceneCamera::ProjectionType)i);
				});

				switch (camera->GetProjectionType())
				{
					case SceneCamera::ProjectionType::Perspective:
					{
						float perspecVerticalFOV = camera->GetPerspectiveVerticalFOV();
						if (UILibrary::DrawFloatControl("Vertical FOV", perspecVerticalFOV, 45.0f, 0.5f, 0.0f, 0.0f))
							camera->SetPerspectiveVerticalFOV(perspecVerticalFOV);

						float perspecNearClip = camera->GetPerspectiveNearClip();
						if (UILibrary::DrawFloatControl("Near Clip", perspecNearClip, 0.01f, 0.1f, 0.0f, 0.0f))
							camera->SetPerspectiveNearClip(perspecNearClip);

						float perspecFarClip = camera->GetPerspectiveFarClip();
						if (UILibrary::DrawFloatControl("Far Clip", perspecFarClip, 10000.0f, 1.0f, 0.0f, 0.0f))
							camera->SetPerspectiveFarClip(perspecFarClip);

						break;
					}
					case SceneCamera::ProjectionType::Orthographic:
					{
						float orthoSize = camera->GetOrthographicSize();
						if (UILibrary::DrawFloatControl("Size", orthoSize, 10.0f, 0.1f, 0.0f, 0.0f))
							camera->SetOrthographicSize(orthoSize);

						float orthoNearClip = camera->GetOrthographicNearClip();
						if (UILibrary::DrawFloatControl("Near Clip", orthoNearClip, -1.0f, 0.01f, 0.0f, 0.0f))
							camera->SetOrthographicNearClip(orthoNearClip);

						float orthoFarClip = camera->GetOrthographicFarClip();
						if (UILibrary::DrawFloatControl("Far Clip", orthoFarClip, 1.0f, 0.01f, 0.0f, 0.0f))
							camera->SetOrthographicFarClip(orthoFarClip);

						break;
					}
					default:
					{
						ME_CORE_ASSERT(false, "Unknown ProjectionType");
						break;
					}
				}

				UILibrary::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);

				bool isActiveCamera = EditorState::GetActiveScene()->GetActiveCamera() == entity;
				if (UILibrary::Checkbox("Active Camera", &isActiveCamera, true))
				{
					if (isActiveCamera)
						EditorState::GetActiveScene()->SetActiveCamera(entity);
					else
						EditorState::GetActiveScene()->SetActiveCamera({});
				}
			});

			// -- Sprite Renderer Component --
			DrawComponent<SpriteRendererComponent>("Sprite Renderer", selectionContext, [](Entity& entity, SpriteRendererComponent& component)
			{
				UILibrary::DrawColor4Control("Color", component.Color);

				glm::vec2 tileFactor = component.TileFactor;
				if (UILibrary::DrawFloat2Control("Tile Factor", "U", "V", tileFactor, 1.0f, 0.1f, true))
					component.TileFactor = tileFactor;

				{
					ImGuiIO& io = ImGui::GetIO();

					ImGui::PushID("Texture");

					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, UILibrary::s_FirstColumnWidth);
					ImGui::Text("Texture");
					ImGui::NextColumn();

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 6 });
					//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 2 });

					float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

					void* previewTexturePtr = nullptr;
					float previewTileFactor = 1.0f;
					if (component.Texture)
					{
						previewTexturePtr = (void*)(uint64_t)component.Texture->GetRendererID();
					}
					else
					{
						previewTexturePtr = (void*)(uint64_t)EditorState::GetTextureLibrary().Checkerboard->GetRendererID();
						previewTileFactor = 5.0f;
					}

					ImGui::Image(previewTexturePtr, { 128.0f, 128.0f }, { 0, previewTileFactor }, { previewTileFactor, 0 });

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_TEXTURE"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path texturePath = std::filesystem::path(g_ContentPath) / path;
							Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
							if (texture->Loaded())
								component.Texture = texture;
							else
								ME_CORE_LOG_WARN("Could not load texture {0}", texturePath.filename().string());
						}

						ImGui::EndDragDropTarget();
					}

					ImGui::PopStyleVar();

					ImGui::Columns(1);

					ImGui::PopID();
				}
			});

			// -- Circle Renderer Component --
			DrawComponent<CircleRendererComponent>("Circle Renderer", selectionContext, [](Entity& entity, CircleRendererComponent& component)
			{
				UILibrary::DrawColor4Control("Color", component.Color);
				UILibrary::DrawFloatControl("Thickness", component.Thickness, 1.0f, 0.025f, 0.0f, 1.0f);
				UILibrary::DrawFloatControl("Fade", component.Fade, 0.005f, 0.00025f, 0.0f, 1.0f, "%.5f");
			});

			// -- Rigid Body 2D Component --
			DrawComponent<Rigidbody2DComponent>("Rigid Body 2D", selectionContext, [](Entity& entity, Rigidbody2DComponent& component)
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic"};
				UILibrary::DrawCombo("Projection", bodyTypeStrings, 3, (int)component.Type, component, [](Rigidbody2DComponent& component, uint32_t i)
				{
					component.Type = (Rigidbody2DComponent::BodyType)i;
				});

				UILibrary::Checkbox("Fixed Rotation", &component.FixedRotation, true);
			});

			// -- Box Collider 2D Component --
			DrawComponent<BoxCollider2DComponent>("Box Collider 2D", selectionContext, [](Entity& entity, BoxCollider2DComponent& component)
			{
				UILibrary::DrawFloat2Control("Size", "X", "Y", component.Size, 0.5f, 0.01f);
				UILibrary::DrawFloat2Control("Offset", "X", "Y", component.Offset, 0.0f, 0.1f);
				UILibrary::DrawFloatControl("Density", component.Density, 1.0f, 0.01f, 0.0f, 1.0f);
				UILibrary::DrawFloatControl("Friction", component.Friction, 0.5f, 0.01f, 0.0f, 1.0f);
				UILibrary::DrawFloatControl("Restitution", component.Restitution, 0.0f, 0.01f, 0.0f, 1.0f);
				UILibrary::DrawFloatControl("Restitution Threshold", component.RestitutionThreshold, 0.5f, 0.01f, 0.0f, 0.0f, "%.2f", true);
			});

			// -- Circle Collider 2D Component --
			DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", selectionContext, [](Entity& entity, CircleCollider2DComponent& component)
			{
				UILibrary::DrawFloat2Control("Offset", "X", "Y", component.Offset, 0.0f, 0.01f);
				UILibrary::DrawFloatControl("Radius", component.Radius, 0.5f, 0.01f);
				UILibrary::DrawFloatControl("Density", component.Density, 1.0f, 0.01f, 0.0f, 1.0f);
				UILibrary::DrawFloatControl("Friction", component.Friction, 0.5f, 0.01f, 0.0f, 1.0f);
				UILibrary::DrawFloatControl("Restitution", component.Restitution, 0.0f, 0.01f, 0.0f, 1.0f);
				UILibrary::DrawFloatControl("Restitution Threshold", component.RestitutionThreshold, 0.5f, 0.01f, 0.0f, 0.0f, "%.2f", true);
			});
		}

		ImGui::End();
	}

}
