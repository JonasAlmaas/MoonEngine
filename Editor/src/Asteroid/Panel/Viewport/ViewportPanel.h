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

		inline glm::vec2 GetMinBound() const { return m_MinBound; }
		inline glm::vec2 GetMaxBound() const { return m_MaxBound; }

		virtual inline bool GetFocused() const { return m_Focused; }
		virtual inline bool GetHovered() const { return m_Hovered; }

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	private:
		glm::vec2 m_Size = { 0.0f, 0.0f };

		glm::vec2 m_MinBound;
		glm::vec2 m_MaxBound;

		bool m_Focused = false;
		bool m_Hovered = false;

		Entity m_HoveredEntity;

		int m_GizmoType = -1;

	};

}
