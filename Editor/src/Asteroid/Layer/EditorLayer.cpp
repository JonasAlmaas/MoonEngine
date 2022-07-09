#include "aopch.h"
#include "Asteroid/Layer/EditorLayer.h"

#include "Asteroid/State/EditorState.h"


namespace Asteroid {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		ME_PROFILE_FUNCTION();

		EditorState::Init();

		// ---- Panel::OnAttach ----

		m_ContentBrowserPanel.OnAttach();
		m_PropertiesPanel.OnAttach();
		m_RendererPanel.OnAttach();
		m_SceneHierarchyPanel.OnAttach();
		m_ToolbarPanel.OnAttach();
		m_ViewportPanel.OnAttach();

		// -------------------------
	}

	void EditorLayer::OnDetach()
	{
		ME_PROFILE_FUNCTION();

		// ---- Panel::OnDetach ----

		m_ContentBrowserPanel.OnDetach();
		m_PropertiesPanel.OnDetach();
		m_RendererPanel.OnDetach();
		m_SceneHierarchyPanel.OnDetach();
		m_ToolbarPanel.OnDetach();
		m_ViewportPanel.OnDetach();

		// -------------------------
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		ME_PROFILE_FUNCTION();

		// ---- Panel::OnUpdate ----

		auto panelState = EditorState::GetPanelState();

		if (panelState->ContentBrowser)
			m_ContentBrowserPanel.OnUpdate(ts);

		if (panelState->Properties)
			m_PropertiesPanel.OnUpdate(ts);

		if (panelState->Renderer)
			m_RendererPanel.OnUpdate(ts);

		if (panelState->SceneHierarchy)
			m_SceneHierarchyPanel.OnUpdate(ts);

		if (panelState->Toolbar)
			m_ToolbarPanel.OnUpdate(ts);

		if (panelState->Viewport)
			m_ViewportPanel.OnUpdate(ts);

		// ---- Render ----

		EditorState::GetFramebuffer()->Bind();

		Renderer2D::ResetStats();
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();

		// Clear entity ID attachment to -1
		EditorState::GetFramebuffer()->ClearAttachment(1, -1);

		SceneState sceneState = EditorState::GetSceneState();
		switch (sceneState)
		{
			case SceneState::Edit:
			{
				if (m_ViewportPanel.GetHovered() || m_ViewportPanel.GetFocused())
					EditorState::GetEditorCamera()->OnUpdate(ts);

				EditorState::GetActiveScene()->OnUpdateEditor(ts, EditorState::GetEditorCamera());
				break;
			}
			case SceneState::Play:
			{
				EditorState::GetActiveScene()->OnRuntimeUpdate(ts);
				break;
			}
		}

		EditorState::GetFramebuffer()->Unbind();
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
		else
		{
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

			style.WindowMinSize.x = 20.0f;
			style.WindowMinSize.y = 20.0f;

			// Submit the DockSpace
			ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

			// ---- Menu Bar ----
			m_MenuBar.OnImGuiRender();

			// ---- Panels::OnImGuiRender ----

			auto panelState = EditorState::GetPanelState();

			if (panelState->ContentBrowser)
				m_ContentBrowserPanel.OnImGuiRender();

			if (panelState->Properties)
				m_PropertiesPanel.OnImGuiRender();
			
			if (panelState->Renderer)
				m_RendererPanel.OnImGuiRender();

			if (panelState->SceneHierarchy)
				m_SceneHierarchyPanel.OnImGuiRender();

			if (panelState->Toolbar)
				m_ToolbarPanel.OnImGuiRender();

			if (panelState->Viewport)
				m_ViewportPanel.OnImGuiRender();

			// -------------------------------

			ImGui::Begin("Statistics");
			{
				auto stats = Renderer2D::GetStats();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Circles: %d", stats.CircleCount);
				ImGui::Text("Lines: %d", stats.LineCount);
				ImGui::Text("Sprites: %d", stats.SpriteCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

				ImGui::End();
			}

			ImGui::End();
		}
	}

	void EditorLayer::DuplicateSelectionContext()
	{
		if (EditorState::GetSceneState() != SceneState::Edit)
			return;

		Ref<EditorScene> activeScene = EditorState::GetActiveScene();
		Entity selectionContext = activeScene->GetSelectionContext();
		if (selectionContext)
			activeScene->DuplicateEntity(selectionContext);
	}

	// ---- Event Handling ----

	void EditorLayer::OnEvent(Event& e)
	{
		ME_PROFILE_FUNCTION();

		// Distribute events

		m_MenuBar.OnEvent(e);

		auto panelState = EditorState::GetPanelState();

		if (panelState->ContentBrowser)
			m_ContentBrowserPanel.OnEvent(e);

		if (panelState->Viewport)
			m_ViewportPanel.OnEvent(e);

		EditorState::GetEditorCamera()->OnEvent(e);

		// Handle events localy

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(ME_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool ctrlPressed = (Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl));
		bool shiftPressed = (Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift));

		switch (e.GetKeyCode())
		{
		case Key::D:
		{
			if (ctrlPressed)
				DuplicateSelectionContext();
		}
		default:
			break;
		}

		return false;
	}

}
