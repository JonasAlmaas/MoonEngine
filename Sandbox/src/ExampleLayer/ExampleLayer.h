#pragma once

#include <Moon.h>

using namespace Moon;


class ExampleLayer : public Layer
{
public:
	ExampleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event& e) override;

private:
	OrthographicCameraController m_CameraController;

	ShaderLibrary m_ShaderLibrary;

	Ref<VertexArray> m_SquareVA;
	Ref<VertexArray> m_TriangleVA;

	Ref<Texture2D> m_Texture;
	Ref<Texture2D> m_TransparentTexture;

	glm::vec4 m_Color1 = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_Color2 = { 0.3f, 0.2f, 0.8f, 1.0f };

};