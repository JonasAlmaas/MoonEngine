#include "aopch.h"
#include "Asteroid/Panel/Viewport/ViewportPanel.h"

#include "Asteroid/State/EditorState.h"

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

		// Resize framebuffer
		if (FramebufferSpecification spec = EditorState::GetFramebuffer()->GetSpecification(); m_Size.x > 0.0f && m_Size.y > 0.0f && (spec.Width != m_Size.x || spec.Height != m_Size.y))
		{
			EditorState::GetFramebuffer()->Resize((uint32_t)m_Size.x, (uint32_t)m_Size.y);
		}
	}

	void ViewportPanel::OnImGuiRender()
	{
		ME_PROFILE_FUNCTION();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");

		m_Focused = ImGui::IsWindowFocused();
		m_Hovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_Focused && !m_Hovered);

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		m_Size = { panelSize.x, panelSize.y };

		ImGui::Image((void*)(uint64_t)EditorState::GetFramebuffer()->GetColorAttachmentRendererID(), panelSize, { 0, 1 }, { 1, 0 });


		// ---- Gizmos ----

		Entity selectedEntity = EditorState::GetActiveScene()->GetSelectionContext();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera
			auto cameraEntity = EditorState::GetActiveScene()->GetActiveCamera();
			if (cameraEntity)
			{
				const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				const glm::mat4& cameraProjection = camera.GetProjection();
				glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

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
			case Key::Q: { m_GizmoType = -1; break; }
			case Key::W: { m_GizmoType = ImGuizmo::OPERATION::TRANSLATE; break; }
			case Key::E: { m_GizmoType = ImGuizmo::OPERATION::ROTATE; break; }
			case Key::R: { m_GizmoType = ImGuizmo::OPERATION::SCALE; break; }
			default:
				break;
		}

		return false;
	}

}
