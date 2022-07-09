#include "aopch.h"
#include "Asteroid/Panel/Toolbar/ToolbarPanel.h"

#include "Asteroid/State/EditorState.h"


namespace Asteroid {

	void ToolbarPanel::OnAttach()
	{
		m_IconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		m_IconStop = Texture2D::Create("Resources/Icons/StopButton.png");
		m_IconSimulate = Texture2D::Create("Resources/Icons/SimulateButton.png");
	}

	void ToolbarPanel::OnDetach()
	{
	}

	void ToolbarPanel::OnUpdate(Timestep ts)
	{
	}

	void ToolbarPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		auto& colors = ImGui::GetStyle().Colors;

		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));

		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;

		SceneState sceneState = EditorState::GetSceneState();

		switch (sceneState)
		{
			case SceneState::Edit:
			{
				ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - size - (size * 0.5f));
				if (ImGui::ImageButton((ImTextureID)m_IconPlay->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
					EditorState::OnScenePlay();

				ImGui::SameLine();

				if (ImGui::ImageButton((ImTextureID)m_IconSimulate->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
					EditorState::OnSimulateStart();

				break;
			}
			case SceneState::Play:
			{
				ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

				if (ImGui::ImageButton((ImTextureID)m_IconStop->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
					EditorState::OnSceneStop();

				break;
			}
			case SceneState::Simulate:
			{
				ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

				if (ImGui::ImageButton((ImTextureID)m_IconStop->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
					EditorState::OnSimulateStop();

				break;
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		ImGui::End();
	}

}
