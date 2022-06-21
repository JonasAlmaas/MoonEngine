#pragma once

using namespace Moon;

#include "Asteroid/Panel/Editor/Viewport/EditorViewportPanel.h"


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
		// -- Panels --
		EditorViewportPanel m_EditorViewportPanel;

		Ref<Scene> m_ActiveScene;

		Entity m_CameraEntity;
		Entity m_SquareEntity;

		Ref<Texture2D> m_Texture_Checkerboard;
		Ref<Texture2D> m_Texture_ColorGrid;

	};

}
