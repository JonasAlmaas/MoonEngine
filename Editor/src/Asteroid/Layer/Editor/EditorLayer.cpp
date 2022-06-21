#include "aopch.h"
#include "Asteroid/Layer/Editor/EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Asteroid {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		ME_PROFILE_FUNCTION();

		// Set up scene
		m_ActiveScene = CreateRef<Scene>();

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>();
		m_ActiveScene->SetActiveCamera(m_CameraEntity);

		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(Color(0.0, 1.0f, 0.0));

		// Generate checkerboard texture
		{
			ME_PROFILE_SCOPE("GenerateTexture-Checkerboard");

			uint32_t* textureData = new uint32_t[2 * 2]{ 0xffcccccc, 0xffffffff, 0xffffffff, 0xffcccccc };
			m_Texture_Checkerboard = Texture2D::Create(2, 2);
			m_Texture_Checkerboard->SetData(textureData, sizeof(uint32_t) * 2 * 2);
			delete[] textureData;
		}

		// Generate color grid texture
		{
			ME_PROFILE_SCOPE("GenerateTexture-ColorGrid");

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

		// ---- Native script test ----
		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				
				if (Input::IsKeyPressed(Key::W))
					transform[3][1] += speed * ts;

				if (Input::IsKeyPressed(Key::S))
					transform[3][1] -= speed * ts;

				if (Input::IsKeyPressed(Key::A))
					transform[3][0] -= speed * ts;

				if (Input::IsKeyPressed(Key::D))
					transform[3][0] += speed * ts;
			}

		private:
			float speed = 5.0f;

		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		// ---- Panel::OnAttach ----

		m_EditorViewportPanel.OnAttach();

		// -------------------------
	}

	void EditorLayer::OnDetach()
	{
		ME_PROFILE_FUNCTION();

		// ---- Panel::OnDetach ----

		m_EditorViewportPanel.OnDetach();

		// -------------------------
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		ME_PROFILE_FUNCTION();

		// ---- Panel::OnUpdate ----
		
		m_EditorViewportPanel.OnUpdate(ts);

		// -------------------------

		glm::vec2 viewportSize = m_EditorViewportPanel.GetSize();
		m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);

		// ---- Render ----
		m_EditorViewportPanel.GetFramebuffer()->Bind();

		m_ActiveScene->OnUpdate(ts);

		m_EditorViewportPanel.GetFramebuffer()->Unbind();
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

			// ---- Panels::OnImGuiRender ----
			
			m_EditorViewportPanel.OnImGuiRender();

			// -------------------------------

			ImGui::Begin("Statistics");
			{
				auto stats = Renderer2D::GetStats();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

				ImGui::End();
			}

			ImGui::Begin("Scene Hierarchy");
			{
				auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
				ImGui::Text("%s", tag.c_str());

				ImGui::ColorEdit4("Square Color", m_SquareEntity.GetComponent<SpriteRendererComponent>().Color);

				ImGui::End();
			}

			ImGui::End();
		}
	}

	// ---- Event Handling ----

	void EditorLayer::OnEvent(Event& e)
	{
		ME_PROFILE_FUNCTION();
	}

}