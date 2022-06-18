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
		// 2x2 images generated on attach
		Ref<Texture2D> m_Texture_Checkerboard;

		//m_EditorCamera;

	};

}
