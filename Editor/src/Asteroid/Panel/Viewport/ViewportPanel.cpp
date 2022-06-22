#include "aopch.h"
#include "Asteroid/Panel/Viewport/ViewportPanel.h"

#include "Asteroid/State/EditorState.h"


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

		Application::Get().GetImGuiLayer()->SetBlockEvents(!(m_Focused && m_Hovered));

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		m_Size = { panelSize.x, panelSize.y };

		ImGui::Image((void*)(uint64_t)EditorState::GetFramebuffer()->GetColorAttachmentRendererID(), panelSize, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

}
