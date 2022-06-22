#include "aopch.h"
#include "Asteroid/Panel/Properties/PropertiesPanel.h"

#include "Asteroid/State/EditorState.h"


namespace Asteroid {
	
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

		ImGui::End();
	}

}
