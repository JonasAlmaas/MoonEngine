#pragma once

using namespace Moon;

#include "Asteroid/MenuBar.h"
#include "Asteroid/Panel/ContentBrowserPanel.h"
#include "Asteroid/Panel/PropertiesPanel.h"
#include "Asteroid/Panel/RendererPanel.h"
#include "Asteroid/Panel/SceneHierarchyPanel.h"
#include "Asteroid/Panel/SettingsPanel.h"
#include "Asteroid/Panel/ToolbarPanel.h"
#include "Asteroid/Panel/ViewportPanel.h"


namespace Asteroid {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		void OnOverlayRender();
		void DuplicateSelectionContext();

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e);

	private:
		MenuBar m_MenuBar;

		// -- Panels --
		ContentBrowserPanel m_ContentBrowserPanel;
		PropertiesPanel m_PropertiesPanel;
		RendererPanel m_RendererPanel;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		SettingsPanel m_SettingsPanel;
		ToolbarPanel m_ToolbarPanel;
		ViewportPanel m_ViewportPanel;

	};

}
