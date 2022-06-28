#pragma once

using namespace Moon;

#include "Asteroid/Panel/Panel.h"


namespace Asteroid {

	class ViewportPanel : public Panel
	{
	public:
		ViewportPanel() = default;
		virtual ~ViewportPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		inline glm::vec2 GetSize() const { return m_Size; }

		virtual inline bool GetFocused() const { return m_Focused; }
		virtual inline bool GetHovered() const { return m_Hovered; }

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		glm::vec2 m_Size = { 0.0f, 0.0f };

		bool m_Focused = false;
		bool m_Hovered = false;

		int m_GizmoType = -1;

	};

}
