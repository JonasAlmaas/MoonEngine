#include "Editor/Layer/Editor/EditorLayer.h"


namespace Asteroid {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), true, true, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		ME_PROFILE_FUNCTION();

		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);

		// Generate checkerboard texture
		{
			uint32_t* textureData = new uint32_t[2 * 2]{ 0xffcccccc, 0xffffffff, 0xffffffff, 0xffcccccc };
			m_Texture_Checkerboard = Texture2D::Create(2, 2);
			m_Texture_Checkerboard->SetData(textureData, sizeof(uint32_t) * 2 * 2);
			delete[] textureData;
		}

		// Generate color grid texture
		{
			uint32_t width = 16;
			uint32_t height = 16;
			uint32_t* textureData = new uint32_t[width * height];
			for (uint8_t x = 0; x < width; x++)
			{
				for (uint8_t y = 0; y < height; y++)
				{
					uint8_t r = (uint8_t)((1.0f - (float)(x + 1) / (float)width) * 255.0f);

					uint8_t g = 0;
					float g1 = (float)(x - y) / (float)(width + height - 2) * 255.0f;
					if (g1 > 0.0f)
						g = (uint8_t)g1;

					uint8_t b = (uint8_t)(((float)(y + 1) / (float)height) * 255.0f);

					textureData[x + y * width] = r + (g << 8) + (b << 16) | 0xff000000;
				}
			}
			m_Texture_ColorGrid = Texture2D::Create(width, height);
			m_Texture_ColorGrid->SetData(textureData, sizeof(uint32_t) * width * height);
			delete[] textureData;
		}
	}

	void EditorLayer::OnDetach()
	{
		ME_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		ME_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(ts);

		Renderer2D::ResetStats();

		{
			ME_PROFILE_SCOPE("Render Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear();
		}

		Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			Renderer2D::DrawQuad({ 0, 0 }, 5, m_Texture_Checkerboard, 16);
		}
		Renderer2D::EndScene();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		ME_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool optFullscreen = true;
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (optFullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
			dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}
		
		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			windowFlags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Asteroid", &dockspaceOpen, windowFlags);
		{
			ImGui::PopStyleVar();

			if (optFullscreen)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();

			style.WindowMinSize.x = 200.0f;
			style.WindowMinSize.y = 200.0f;

			// Submit the DockSpace
			ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit", "Alt+F4"))
						Application::Get().Close();

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::Begin("Statistics");
			{
				auto stats = Renderer2D::GetStats();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

				ImGui::Image((void*)m_Texture_Checkerboard->GetRendererID(), { 256, 256 }, { 0, 0 }, { 5, 5 });
				ImGui::Image((void*)m_Texture_ColorGrid->GetRendererID(), { 256, 256 });
				
				ImGui::End();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
			ImGui::Begin("Viewport");
			{
				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();

				Application::Get().GetImGuiLayer()->SetBlockEvents(!(m_ViewportFocused && m_ViewportHovered));

				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				if (((m_ViewportSize.x != viewportPanelSize.x) || (m_ViewportSize.y != viewportPanelSize.y)) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
				{
					m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
					m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
					m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				}

				ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), viewportPanelSize, { 0, 1 }, { 1, 0 });

				ImGui::End();
			}
			ImGui::PopStyleVar();

			ImGui::End();
		}
	}

	// ---- Event Handling ----

	void EditorLayer::OnEvent(Event& e)
	{
		ME_PROFILE_FUNCTION();

		m_CameraController.OnEvent(e);
	}

}
