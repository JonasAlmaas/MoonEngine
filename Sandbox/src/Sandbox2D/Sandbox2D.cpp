#include "Sandbox2D/Sandbox2D.h"

#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), false, true, true)
{

}

void Sandbox2D::OnAttach()
{
	m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Timestep ts)
{
	// ---- Update ----
	m_CameraController.OnUpdate(ts);

	// ---- Render ----
	RenderCommand::SetClearColor({ ColorFormat::RGBADecimal, 25 });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_CameraController.GetCamera());

	float offset = 1.1f;

	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			if ((x % 2 == 0 && y % 2 == 0) || (x % 2 != 0 && y % 2 != 0))
			{
				Renderer2D::DrawQuad({ x * offset, y * offset }, { 1.0f, 1.0f }, m_Color1);
			}
			else {
				Renderer2D::DrawQuad({ x * offset, y * offset }, { 1.0f, 1.0f }, m_Color2);
			}
		}
	}

	Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1 }, 45.0f, 10.0f, m_Texture, 10.0f);

	Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("2D Settings");

	ImGui::ColorEdit3("Color 1", glm::value_ptr(m_Color1));
	ImGui::ColorEdit3("Color 2", glm::value_ptr(m_Color2));

	ImGui::End();
}

// ---- Event Handling ----

void Sandbox2D::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}
