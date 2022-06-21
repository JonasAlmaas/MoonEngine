#pragma once

using namespace Moon;

#include "Asteroid/Panel/Panel.h"


namespace Asteroid {

	class EditorViewportPanel : public Panel
	{
	public:
		EditorViewportPanel() = default;
		virtual ~EditorViewportPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;

		inline Ref<Framebuffer> GetFramebuffer() const { return m_Framebuffer; }
		
		inline glm::vec2 GetSize() const { return m_Size; }

		virtual inline bool GetFocused() const { return m_Focused; }
		virtual inline bool GetHovered() const { return m_Hovered; }

	private:
		Ref<Framebuffer> m_Framebuffer;
		
		glm::vec2 m_Size = { 0.0f, 0.0f };

		bool m_Focused = false;
		bool m_Hovered = false;

	};


}
