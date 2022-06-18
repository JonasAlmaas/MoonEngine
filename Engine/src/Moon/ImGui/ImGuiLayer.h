#pragma once

#include "Moon/Core/Layer/Layer.h"

#include "Moon/Core/Event/ApplicationEvent.h"
#include "Moon/Core/Event/KeyEvent.h"
#include "Moon/Core/Event/MouseEvent.h"


namespace Moon {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }

	private:
		void SetStyle();

	private:
		bool m_BlockEvents = true;

	};

}
