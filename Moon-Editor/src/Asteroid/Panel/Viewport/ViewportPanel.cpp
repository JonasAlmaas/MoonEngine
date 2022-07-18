#include "aopch.h"
#include "Asteroid/Panel/Viewport/ViewportPanel.h"

#include "Asteroid/State/EditorState.h"

#include <Moon/Scene/Serializer/SceneSerializer.h>
#include <ImGuizmo.h>


namespace Asteroid {

	extern const std::filesystem::path g_ContentPath;

	void ViewportPanel::OnAttach()
	{
		ME_PROFILE_FUNCTION();
	}

	void ViewportPanel::OnDetach()
	{
		ME_PROFILE_FUNCTION();

	}

	void ViewportPanel::OnUpdate(Timestep ts)
	{
		ME_PROFILE_FUNCTION();

		// -- Resize --
		if (FramebufferSpecification spec = EditorState::GetFramebuffer()->GetSpecification(); m_Size.x > 0.0f && m_Size.y > 0.0f && (spec.Width != m_Size.x || spec.Height != m_Size.y))
		{
			EditorState::GetFramebuffer()->Resize((uint32_t)m_Size.x, (uint32_t)m_Size.y);
			EditorState::GetActiveScene()->OnViewportResize((uint32_t)m_Size.x, (uint32_t)m_Size.y);
			EditorState::GetEditorCamera()->SetViewportSize((float)m_Size.x, (float)m_Size.y);
		}

		// Entity picking
		EditorState::GetFramebuffer()->Bind();

		auto [mx, my] = ImGui::GetMousePos();

		mx -= m_MinBound.x;
		my -= m_MinBound.y;

		glm::vec2 viewportSize = m_MaxBound - m_MinBound;
		// Flip y
		my = viewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = EditorState::GetFramebuffer()->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = (pixelData != -1) ? Entity(&EditorState::GetActiveScene()->GetRegistry(), (uint32_t)pixelData) : Entity();
		}

		EditorState::GetFramebuffer()->Unbind();
	}

	void ViewportPanel::OnImGuiRender()
	{
		ME_PROFILE_FUNCTION();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");

		// Get information about the current imgui window, used for selecting from the image
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_MinBound = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_MaxBound = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_Focused = ImGui::IsWindowFocused();
		m_Hovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_Focused && !m_Hovered);

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		m_Size = { panelSize.x, panelSize.y };

		ImGui::Image((void*)(uint64_t)EditorState::GetFramebuffer()->GetColorAttachmentRendererID(), panelSize, { 0, 1 }, { 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_MAP"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				EditorState::OpenScene(std::filesystem::path(g_ContentPath) / path);
			}

			ImGui::EndDragDropTarget();
		}

		// Don't allow a lot of fucntionality if the user is moving
		if (!Input::IsKeyPressed(Key::LeftAlt))
		{
			Ref<EditorScene> scene = EditorState::GetActiveScene();

			// Check if there are any entities in the scene
			if (scene->GetRegistry().size() > 0)
			{
				// ---- Gizmos ----

				Entity selectedEntity = scene->GetSelectionContext();
				if (selectedEntity && m_GizmoType != -1)
				{
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();

					ImGuizmo::SetRect(m_MinBound.x, m_MinBound.y, m_MaxBound.x - m_MinBound.x, m_MaxBound.y - m_MinBound.y);

					// Get active camera
					glm::mat4 cameraView;
					const float* cameraProjection = nullptr;

					if (EditorState::GetSceneState() == SceneState::Edit)
					{
						auto camera = EditorState::GetEditorCamera();
						cameraProjection = glm::value_ptr(camera->GetProjection());
						cameraView = camera->GetView();
					}
					else
					{
						auto cameraEntity = scene->GetActiveCamera();
						if (cameraEntity)
						{
							const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
							cameraProjection = glm::value_ptr(camera->GetProjection());
							cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
						}
					}

					// Make sure we have an active camera
					if (cameraProjection)
					{
						// Entity Transform
						auto& tc = selectedEntity.GetComponent<TransformComponent>();
						glm::mat4 transform = tc.GetTransform();

						// Snapping
						bool snap = Input::IsKeyPressed(Key::LeftControl);
						float snapValue = 0.5f;

						if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
							snapValue = 15.0f;

						float snapValues[] = { snapValue, snapValue, snapValue };

						ImGuizmo::Manipulate(glm::value_ptr(cameraView), cameraProjection, (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

						if (ImGuizmo::IsUsing())
						{
							glm::vec3 translation, rotation, scale;
							Math::DecomposeTransform(transform, translation, rotation, scale);

							tc.Translation = translation;

							glm::vec3 deltaRotation = rotation - tc.Rotation;
							tc.Rotation += deltaRotation;

							tc.Scale = scale;
						}
					}
				}
			}
			else
			{
				// Allow the user to click the background to open a file
				// Only if there isn't anything in the current scene
				if (Input::IsMouseButtonPressed(Mouse::Button0) && m_Hovered)
				{
					// Make sure the click is not on the tabbar
					ImVec2 mousePos = ImGui::GetMousePos();
					if (mousePos.x > m_MinBound.x && mousePos.y > m_MinBound.y && mousePos.x < m_MaxBound.x && mousePos.y < m_MaxBound.y)
						EditorState::OpenScene();
				}
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	// ---- Event Handling ----

	void ViewportPanel::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressedEvent>(ME_BIND_EVENT_FN(ViewportPanel::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(ME_BIND_EVENT_FN(ViewportPanel::OnMouseButtonPressed));
	}

	bool ViewportPanel::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;

		switch (e.GetKeyCode())
		{
			case Key::Q:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = -1;
				break;
			}
			case Key::W:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
			default:
				break;
		}

		return false;
	}

	bool ViewportPanel::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_Hovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				EditorState::GetActiveScene()->SetSelectionContext(m_HoveredEntity);
		}

		return false;
	}

}
