#include "Sandbox2D/Sandbox2D.h"


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), false, true, true)
{
}

void Sandbox2D::OnAttach()
{
	ME_PROFILE_FUNCTION();

	m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	ME_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Timestep ts)
{
	ME_PROFILE_FUNCTION();

	// ---- Update ----
	m_CameraController.OnUpdate(ts);

	// ---- Render ----
	RenderCommand::SetClearColor({ ColorFormat::RGBADecimal, 25 });
	RenderCommand::Clear();

	Renderer2D::ResetStats();

	Renderer2D::BeginScene(m_CameraController.GetCamera());

	Renderer2D::DrawQuad({ 10.0f, 10.0f, -0.1 }, 5.0f, m_Texture, 2.0f);

	float offset = 1.1f;
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			if ((x % 2 == 0 && y % 2 == 0) || (x % 2 != 0 && y % 2 != 0))
			{
				Renderer2D::DrawQuad({ x * offset, y * offset }, 1.0f, m_Texture, m_Color1);
			}
			else {
				Renderer2D::DrawQuad({ x * offset, y * offset }, 1.0f, m_Texture, m_Color2);
			}
		}
	}

	Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1 }, 45.0f, 10.0f, m_Texture, 10.0f);

	Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ME_PROFILE_FUNCTION();

	ImGui::Begin("2D Settings");

	ImGui::ColorEdit4("Color 1", m_Color1.GetValuePtr());
	ImGui::ColorEdit4("Color 2", m_Color2.GetValuePtr());

	auto stats = Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("	Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("	Quad Count: %d", stats.QuadCount);
	ImGui::Text("	Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("	Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}

// ---- Event Handling ----

void Sandbox2D::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}
