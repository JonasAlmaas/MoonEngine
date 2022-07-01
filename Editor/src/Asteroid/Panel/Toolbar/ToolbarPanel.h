#pragma once

using namespace Moon;

#include "Asteroid/Panel/Panel.h"


namespace Asteroid {

	class ToolbarPanel : public Panel
	{
	public:
		ToolbarPanel() = default;
		virtual ~ToolbarPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;

	private:
		Ref<Texture2D> m_IconPlay;
		Ref<Texture2D> m_IconStop;

	};

}
