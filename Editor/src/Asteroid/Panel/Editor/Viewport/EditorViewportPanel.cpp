#include "aopch.h"
#include "Asteroid/Panel/Editor/Viewport/EditorViewportPanel.h"


namespace Asteroid {

	void EditorViewportPanel::OnAttach()
	{
		ME_PROFILE_FUNCTION();

		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);
	}

	void EditorViewportPanel::OnDetach()
	{
		ME_PROFILE_FUNCTION();

	}

	void EditorViewportPanel::OnUpdate(Timestep ts)
	{
		ME_PROFILE_FUNCTION();

		// Resize framebuffer
		m_Framebuffer->Resize((uint32_t)m_Size.x, (uint32_t)m_Size.y);
	}

	void EditorViewportPanel::OnImGuiRender()
	{
		ME_PROFILE_FUNCTION();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");

		m_Focused = ImGui::IsWindowFocused();
		m_Hovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->SetBlockEvents(!(m_Focused && m_Hovered));

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_Size = { viewportPanelSize.x, viewportPanelSize.y };

		ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), viewportPanelSize, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

}
