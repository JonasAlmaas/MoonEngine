#include "aopch.h"
#include "Asteroid/Panel/Settings/SettingsPanel.h"

#include "Asteroid/Panel/UILibrary/UILibrary.h"
#include "Asteroid/State/EditorState.h"


namespace Asteroid {

	void SettingsPanel::OnAttach()
	{
	}

	void SettingsPanel::OnDetach()
	{
	}

	void SettingsPanel::OnUpdate(Timestep ts)
	{
	}

	void SettingsPanel::OnImGuiRender()
	{
		ImGui::Begin("Settings");

		bool showPhysicsColliders = EditorState::GetShowPhysicsColliders();
		if (UILibrary::Checkbox("Show Physics Colliders", &showPhysicsColliders))
			EditorState::SetShowPhysicsColliders(showPhysicsColliders);

		ImGui::End();
	}

}
