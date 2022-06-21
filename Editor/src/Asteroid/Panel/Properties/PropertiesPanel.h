#pragma once

using namespace Moon;

#include "Asteroid/Panel/Panel.h"


namespace Asteroid {

	class PropertiesPanel : public Panel
	{
	public:
		PropertiesPanel() = default;
		virtual ~PropertiesPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;

	};

}
