#pragma once

using namespace Moon;

#include "Asteroid/MenuBar/MenuBar.h"
#include "Asteroid/Panel/ContentBrowser/ContentBrowserPanel.h"
#include "Asteroid/Panel/Properties/PropertiesPanel.h"
#include "Asteroid/Panel/Renderer/RendererPanel.h"
#include "Asteroid/Panel/SceneHierarchy/SceneHierarchyPanel.h"
#include "Asteroid/Panel/Toolbar/ToolbarPanel.h"
#include "Asteroid/Panel/Viewport/ViewportPanel.h"


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
		MenuBar m_MenuBar;

		// -- Panels --
		ContentBrowserPanel m_ContentBrowserPanel;
		PropertiesPanel m_PropertiesPanel;
		RendererPanel m_RendererPanel;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ViewportPanel m_ViewportPanel;
	};

}
