#pragma once

#include <Moon.h>

using namespace Moon;


namespace Asteroid {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		OrthographicCameraController m_CameraController;

		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_Texture_Checkerboard;
		Ref<Texture2D> m_Texture_ColorGrid;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

	};

}
