#include "mepch.h"
#include "Moon/ImGui/ImGuiLayer.h"

#include "Moon/Core/Application/Application.h"
#include "Moon/ImGui/Customization/ImGuiColorPalette.h"

// Emedded font
#include "Font/Roboto/Roboto-Bold.embed"
#include "Font/Roboto/Roboto-Regular.embed"

#include <ImGuizmo.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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
		style.WindowMenuButtonPosition = ImGuiDir_None;
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

		style.Colors[ImGuiCol_Text]						= ImGuiColorPalette::Text;
		style.Colors[ImGuiCol_TextDisabled]				= ImGuiColorPalette::TextDisabled;

		style.Colors[ImGuiCol_WindowBg]					= ImGuiColorPalette::WindowBg;					// Background of normal windows
		style.Colors[ImGuiCol_ChildBg]					= ImGuiColorPalette::ChildBg;					// Background of child windows
		style.Colors[ImGuiCol_PopupBg]					= ImGuiColorPalette::PopupBg;					// Background of popups, menus, tooltips windows
		
		style.Colors[ImGuiCol_Border]					= ImGuiColorPalette::Border;
		style.Colors[ImGuiCol_BorderShadow]				= ImGuiColorPalette::BorderShadow;
		
		style.Colors[ImGuiCol_FrameBg]					= ImGuiColorPalette::ElementBg;					// Background of checkbox, radio button, plot, slider, text input
		style.Colors[ImGuiCol_FrameBgHovered]			= ImGuiColorPalette::ElementHovered;
		style.Colors[ImGuiCol_FrameBgActive]			= ImGuiColorPalette::ElementActive;
		
		style.Colors[ImGuiCol_TitleBg]					= ImGuiColorPalette::TitleBg;
		style.Colors[ImGuiCol_TitleBgActive]			= ImGuiColorPalette::TitleBgActive;
		style.Colors[ImGuiCol_TitleBgCollapsed]			= ImGuiColorPalette::TitleBgCollapsed;
		style.Colors[ImGuiCol_MenuBarBg]				= ImGuiColorPalette::MenuBarBg;

		style.Colors[ImGuiCol_ScrollbarBg]				= ImGuiColorPalette::ScrollbarBg;
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImGuiColorPalette::ScrollbarGrab;
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImGuiColorPalette::ScrollbarGrabHovered;
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImGuiColorPalette::ScrollbarGrabActive;

		style.Colors[ImGuiCol_CheckMark]				= ImGuiColorPalette::Primary;
		style.Colors[ImGuiCol_SliderGrab]				= ImGuiColorPalette::Primary;
		style.Colors[ImGuiCol_SliderGrabActive]			= ImGuiColorPalette::PrimaryHover;

		style.Colors[ImGuiCol_Button]					= ImGuiColorPalette::Button;
		style.Colors[ImGuiCol_ButtonHovered]			= ImGuiColorPalette::ButtonHovered;
		style.Colors[ImGuiCol_ButtonActive]				= ImGuiColorPalette::ButtonActive;

		style.Colors[ImGuiCol_Header]					= ImGuiColorPalette::Header;					// Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		style.Colors[ImGuiCol_HeaderHovered]			= ImGuiColorPalette::HeaderHovered;
		style.Colors[ImGuiCol_HeaderActive]				= ImGuiColorPalette::HeaderActive;

		style.Colors[ImGuiCol_Separator]				= ImGuiColorPalette::Separator;
		style.Colors[ImGuiCol_SeparatorHovered]			= ImGuiColorPalette::SeparatorHovered;
		style.Colors[ImGuiCol_SeparatorActive]			= ImGuiColorPalette::SeparatorActive;

		style.Colors[ImGuiCol_ResizeGrip]				= ImGuiColorPalette::ResizeGrip;
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImGuiColorPalette::ResizeGripHovered;
		style.Colors[ImGuiCol_ResizeGripActive]			= ImGuiColorPalette::ResizeGripActive;

		style.Colors[ImGuiCol_Tab]						= ImGuiColorPalette::Tab;
		style.Colors[ImGuiCol_TabHovered]				= ImGuiColorPalette::TabHovered;
		style.Colors[ImGuiCol_TabActive]				= ImGuiColorPalette::TabActive;
		style.Colors[ImGuiCol_TabUnfocused]				= ImGuiColorPalette::TabUnfocused;
		style.Colors[ImGuiCol_TabUnfocusedActive]		= ImGuiColorPalette::TabUnfocusedActive;

		style.Colors[ImGuiCol_DockingPreview]			= ImGuiColorPalette::DockingPreview;			// Preview overlay color when about to docking something
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImGuiColorPalette::DockingEmptyBg;			// Background color for empty node (e.g. CentralNode with no window docked into it)

		style.Colors[ImGuiCol_PlotLines]				= ImGuiColorPalette::PlotLines;
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImGuiColorPalette::PlotLinesHovered;
		style.Colors[ImGuiCol_PlotHistogram]			= ImGuiColorPalette::PlotHistogram;
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImGuiColorPalette::PlotHistogramHovered;

		style.Colors[ImGuiCol_TableHeaderBg]			= ImGuiColorPalette::TableHeaderBg;				// Table header background
		style.Colors[ImGuiCol_TableBorderStrong]		= ImGuiColorPalette::TableBorderStrong;			// Table outer and header borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableBorderLight]			= ImGuiColorPalette::TableBorderLight;			// Table inner borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableRowBg]				= ImGuiColorPalette::TableRowBg;				// Table row background (even rows)
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImGuiColorPalette::TableRowBgAlt;				// Table row background (odd rows)
		
		style.Colors[ImGuiCol_TextSelectedBg]			= ImGuiColorPalette::TextSelectedBg;
		
		style.Colors[ImGuiCol_DragDropTarget]			= ImGuiColorPalette::DragDropTarget;
		style.Colors[ImGuiCol_NavHighlight]				= ImGuiColorPalette::NavHighlight;				// Gamepad/keyboard: current highlighted item

		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImGuiColorPalette::NavWindowingHighlight;		// Highlight window when using CTRL+TAB
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImGuiColorPalette::NavWindowingDimBg;			// Darken/colorize entire screen behind the CTRL+TAB window list, when active
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImGuiColorPalette::ModalWindowDimBg;			// Darken/colorize entire screen behind a modal window, when one is active

		// Load default font
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;

		io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoBold, g_RobotoBoldLen, 18.0f, &fontConfig);

		ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 18.0f, &fontConfig);
		io.FontDefault = robotoFont;
	}

}
