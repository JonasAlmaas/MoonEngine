#pragma once

#include <Moon.h>

using namespace Moon;


namespace Sandbox {

	class Sandbox2D : public Layer
	{
	public:
		Sandbox2D();
		~Sandbox2D() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		OrthographicCameraController m_CameraController;

		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_TextureSheet;
		Ref<SubTexture2D> m_SubTexture;

		Color m_Color1 = { 0.8f, 0.2f, 0.3f, 1.0f };
		Color m_Color2 = { 0.3f, 0.2f, 0.8f, 1.0f };

	};

}
