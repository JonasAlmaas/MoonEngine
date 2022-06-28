#include "aopch.h"
#include "Asteroid/Panel/Viewport/ViewportPanel.h"

#include "Asteroid/State/EditorState.h"

#include <Moon/Scene/Serializer/SceneSerializer.h>
#include <ImGuizmo.h>


namespace Asteroid {

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
		m_ViewportMinBound = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportMaxBound = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_Focused = ImGui::IsWindowFocused();
		m_Hovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_Focused && !m_Hovered);

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		m_Size = { panelSize.x, panelSize.y };

		ImGui::Image((void*)(uint64_t)EditorState::GetFramebuffer()->GetColorAttachmentRendererID(), panelSize, { 0, 1 }, { 1, 0 });

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

				ImGuizmo::SetRect(m_ViewportMinBound.x, m_ViewportMinBound.y, m_ViewportMaxBound.x - m_ViewportMinBound.x, m_ViewportMaxBound.y - m_ViewportMinBound.y);

				// Camera
				//auto cameraEntity = scene->GetActiveCamera();
				//if (cameraEntity)
				//{
				//	const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				//	const glm::mat4& cameraProjection = camera.GetProjection();
				//	glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

				auto camera = EditorState::GetEditorCamera();
				const glm::mat4& cameraProjection = camera->GetProjection();
				glm::mat4 cameraView = camera->GetViewMatrix();

				// Entity Transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();

				// Snapping
				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = 0.5f;

				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 15.0f;

				float snapValues[] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

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
		else
		{
			// Allow the user to click the background to open a file
			// Only if there isn't anything in the current scene
			if (Input::IsMouseButtonPressed(Mouse::Button0) && m_Hovered)
			{
				std::string filepath = FileDialog::OpenFile("Moon Scene (*.mmap)\0*.mmap\0");

				if (!filepath.empty())
				{
					Ref<Scene> activeScene = EditorState::NewActiveScene();
					SceneSerializer serializer(activeScene);
					serializer.Deserialize(filepath);
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
	}

	bool ViewportPanel::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
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

}
