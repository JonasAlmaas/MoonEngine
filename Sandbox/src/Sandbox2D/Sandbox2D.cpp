#include "Sandbox2D/Sandbox2D.h"


namespace Sandbox {

	Sandbox2D::Sandbox2D()
		: Layer("Sandbox2D"), m_CameraController((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), false, true, true)
	{
	}

	void Sandbox2D::OnAttach()
	{
		ME_PROFILE_FUNCTION();

		m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");

		// Setup particle
		m_Particle.Position = { 0.0f, 0.0f };
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.ColorBegin = { ColorFormat::RGBADecimal, 254.0f, 212.0f, 123.0f };
		m_Particle.ColorEnd = { ColorFormat::RGBADecimal, 254.0f, 109.0f, 41.0f };
		m_Particle.SizeBegin = 0.5f;
		m_Particle.SizeEnd = 0.0f;
		m_Particle.SizeVariation = 0.3f;
		m_Particle.LifeTime = 5.0f;
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
		m_ParticleSystem.OnUpdate(ts);

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
		

		if (Input::IsMouseButtonPressed(Mouse::Button0))
		{
			auto [x, y] = Input::GetMousePosition();
			auto width = Application::Get().GetWindow().GetWidth();
			auto height = Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();

			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();

			m_Particle.Position = { x + pos.x, y + pos.y };

			for (int i = 0; i < 25; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
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

}
