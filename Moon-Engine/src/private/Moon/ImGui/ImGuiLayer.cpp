#include "mepch.h"
#include "Moon/ImGui/ImGuiLayer.h"

#include "Moon/Core/Application.h"

// Emedded font
#include "Font/Roboto/Roboto-Bold.embed"
#include "Font/Roboto/Roboto-Regular.embed"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <ImGuizmo.h>

// TEMPORARY
#include <GLFW/glfw3.h>


namespace Moon {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ME_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();

		// Setup ImGui context
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;			// Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;			// Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;				// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;				// Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		SetStyle();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ME_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		ME_PROFILE_FUNCTION();

		if (!m_BlockEvents)
			return;

		ImGuiIO& io = ImGui::GetIO();
		e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}

	void ImGuiLayer::Begin()
	{
		ME_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		ME_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetStyle()
	{
		ME_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		io.ConfigWindowsMoveFromTitleBarOnly = true;

		style.Alpha = 1.0f;
		style.DisabledAlpha = 1.0f;
		style.WindowPadding = ImVec2(10.0f, 10.0f);
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(10.0f, 10.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(10.0f, 3.0f);
		style.FrameRounding = 0.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(10.0f, 8.0f);
		style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
		style.IndentSpacing = 10.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 10.0f;
		style.ScrollbarRounding = 100.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 0.0f;
		style.LogSliderDeadzone = 4.0f;
		style.TabRounding = 0.0f;
		style.TabBorderSize = 0.0f;
		style.TabMinWidthForCloseButton = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
		style.DisplayWindowPadding = ImVec2(19.0f, 19.0f);
		style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);
		style.MouseCursorScale = 1.0f;
		style.AntiAliasedLines = true;
		style.AntiAliasedLinesUseTex = true;
		style.AntiAliasedFill = true;
		style.CurveTessellationTol = 1.25f;
		style.CircleTessellationMaxError = 0.5f;

		style.Colors[ImGuiCol_Text]						= Color(242);
		style.Colors[ImGuiCol_TextDisabled]				= Color(204);

		style.Colors[ImGuiCol_WindowBg]					= Color(42);				// Background of normal windows
		style.Colors[ImGuiCol_ChildBg]					= Color(42);				// Background of child windows
		style.Colors[ImGuiCol_PopupBg]					= Color(42);				// Background of popups, menus, tooltips windows
		
		style.Colors[ImGuiCol_Border]					= Color(56);
		style.Colors[ImGuiCol_BorderShadow]				= Color(0.0f, 0.0f);
		
		style.Colors[ImGuiCol_FrameBg]					= Color(60);				// Background of checkbox, radio button, plot, slider, text input
		style.Colors[ImGuiCol_FrameBgHovered]			= Color(75);
		style.Colors[ImGuiCol_FrameBgActive]			= Color(60);
		
		style.Colors[ImGuiCol_TitleBg]					= Color(26);
		style.Colors[ImGuiCol_TitleBgActive]			= Color(26);
		style.Colors[ImGuiCol_TitleBgCollapsed]			= Color(26);
		style.Colors[ImGuiCol_MenuBarBg]				= Color(26);

		style.Colors[ImGuiCol_ScrollbarBg]				= Color(36);
		style.Colors[ImGuiCol_ScrollbarGrab]			= Color(109);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= Color(149);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= Color(149);

		style.Colors[ImGuiCol_CheckMark]				= Color(46, 104, 148);
		style.Colors[ImGuiCol_SliderGrab]				= Color(46, 104, 148);
		style.Colors[ImGuiCol_SliderGrabActive]			= Color(40, 85, 120);

		style.Colors[ImGuiCol_Button]					= Color(60);
		style.Colors[ImGuiCol_ButtonHovered]			= Color(75);
		style.Colors[ImGuiCol_ButtonActive]				= Color(60);

		style.Colors[ImGuiCol_Header]					= Color(65);				// Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		style.Colors[ImGuiCol_HeaderHovered]			= Color(65);
		style.Colors[ImGuiCol_HeaderActive]				= Color(65);

		style.Colors[ImGuiCol_Separator]				= Color(56);
		style.Colors[ImGuiCol_SeparatorHovered]			= Color(56);
		style.Colors[ImGuiCol_SeparatorActive]			= Color(56);

		style.Colors[ImGuiCol_ResizeGrip]				= Color(65);
		style.Colors[ImGuiCol_ResizeGripHovered]		= Color(46, 104, 148);
		style.Colors[ImGuiCol_ResizeGripActive]			= Color(46, 104, 148);

		style.Colors[ImGuiCol_Tab]						= Color(26);
		style.Colors[ImGuiCol_TabHovered]				= Color(56);
		style.Colors[ImGuiCol_TabActive]				= Color(56);
		style.Colors[ImGuiCol_TabUnfocused]				= Color(26);
		style.Colors[ImGuiCol_TabUnfocusedActive]		= Color(56);

		style.Colors[ImGuiCol_DockingPreview]			= Color(44);				// Preview overlay color when about to docking something
		style.Colors[ImGuiCol_DockingEmptyBg]			= Color(48);				// Background color for empty node (e.g. CentralNode with no window docked into it)

		style.Colors[ImGuiCol_PlotLines]				= Color(156);
		style.Colors[ImGuiCol_PlotLinesHovered]			= Color(255, 110, 89);
		style.Colors[ImGuiCol_PlotHistogram]			= Color(230, 179, 0);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= Color(255, 153, 0);

		style.Colors[ImGuiCol_TableHeaderBg]			= Color(25);				// Table header background
		style.Colors[ImGuiCol_TableBorderStrong]		= Color(65);				// Table outer and header borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableBorderLight]			= Color(50);				// Table inner borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableRowBg]				= Color(0.0f, 0.0f);		// Table row background (even rows)
		style.Colors[ImGuiCol_TableRowBgAlt]			= Color(255, 8);			// Table row background (odd rows)
		
		style.Colors[ImGuiCol_TextSelectedBg]			= Color(46, 104, 148);
		
		style.Colors[ImGuiCol_DragDropTarget]			= Color(85);
		style.Colors[ImGuiCol_NavHighlight]				= Color(85);				// Gamepad/keyboard: current highlighted item

		style.Colors[ImGuiCol_NavWindowingHighlight]	= Color(85);				// Highlight window when using CTRL+TAB
		style.Colors[ImGuiCol_NavWindowingDimBg]		= Color(204, 51);			// Darken/colorize entire screen behind the CTRL+TAB window list, when active
		style.Colors[ImGuiCol_ModalWindowDimBg]			= Color(204, 89);			// Darken/colorize entire screen behind a modal window, when one is active

		// Load default font
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;

		io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoBold, g_RobotoBoldLen, 18.0f, &fontConfig);

		ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 18.0f, &fontConfig);
		io.FontDefault = robotoFont;
	}

}
