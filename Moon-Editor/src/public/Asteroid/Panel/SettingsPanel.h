#pragma once

using namespace Moon;

#include "Asteroid/Panel.h"


namespace Asteroid {

	class SettingsPanel : public Panel
	{
	public:
		SettingsPanel() = default;
		virtual ~SettingsPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;

	};

}
