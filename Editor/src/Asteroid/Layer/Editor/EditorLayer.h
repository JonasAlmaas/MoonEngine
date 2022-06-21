#pragma once

using namespace Moon;


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

		Entity m_CameraEntity;
		Entity m_SquareEntity;

		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		Ref<Texture2D> m_Texture_Checkerboard;
		Ref<Texture2D> m_Texture_ColorGrid;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

	};

}
