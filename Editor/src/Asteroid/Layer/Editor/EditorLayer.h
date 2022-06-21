#pragma once

using namespace Moon;

#include "Asteroid/Panel/Properties/PropertiesPanel.h"
#include "Asteroid/Panel/SceneHierarchy/SceneHierarchyPanel.h"
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
		Ref<Scene> m_ActiveScene;

		// -- Panels --
		PropertiesPanel m_PropertiesPanel;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ViewportPanel m_ViewportPanel;


		Entity m_CameraEntity;
		Entity m_SquareEntity;

		Ref<Texture2D> m_Texture_Checkerboard;
		Ref<Texture2D> m_Texture_ColorGrid;

	};

}
