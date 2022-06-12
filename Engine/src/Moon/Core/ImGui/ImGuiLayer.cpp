#include "mepch.h"
#include "Moon/Core/ImGui/ImGuiLayer.h"

#include "Moon/Customization/UIColor.h"
#include "Moon/Core/Application/Application.h"
#include "Moon/Core/Type/Color.h"

// Emedded font
#include "Font/Roboto-Regular.embed"

#include <imgui.h>
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
		IMGUI_CHECKVERSION();

		// Setup ImGui context
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;			// Enable Keyboard Controls
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
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
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
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		// TODO: This is only ment for 4k monitors, should be a setting you can set in the editor.
		io.FontGlobalScale = 1.5f;

		style.Alpha = 1.0f;
		style.DisabledAlpha = 1.0f;
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(10.0f, 10.0f);
		style.WindowTitleAlign = ImVec2(0, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 4.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(4.0f, 2.0f);
		style.FrameRounding = 4.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(10.0f, 5.0f);
		style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 15.0f;
		style.ScrollbarRounding = 9.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 0.0f;
		style.LogSliderDeadzone = 4.0f;
		style.TabRounding = 4.0f;
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

		style.Colors[ImGuiCol_Text]						= UIColor::Text;
		style.Colors[ImGuiCol_TextDisabled]				= UIColor::TextDisabled;

		style.Colors[ImGuiCol_WindowBg]					= UIColor::WindowBg;				// Background of normal windows
		style.Colors[ImGuiCol_ChildBg]					= UIColor::ChildBg;					// Background of child windows
		style.Colors[ImGuiCol_PopupBg]					= UIColor::PopupBg;					// Background of popups, menus, tooltips windows
		
		style.Colors[ImGuiCol_Border]					= UIColor::Border;
		style.Colors[ImGuiCol_BorderShadow]				= UIColor::ElementOutline;
		
		style.Colors[ImGuiCol_FrameBg]					= UIColor::ElementBg;				// Background of checkbox, radio button, plot, slider, text input
		style.Colors[ImGuiCol_FrameBgHovered]			= UIColor::ElementHovered;
		style.Colors[ImGuiCol_FrameBgActive]			= UIColor::ElementActive;
		
		style.Colors[ImGuiCol_TitleBg]					= UIColor::TitleBg;
		style.Colors[ImGuiCol_TitleBgActive]			= UIColor::TitleBgActive;
		style.Colors[ImGuiCol_TitleBgCollapsed]			= UIColor::TitleBgCollapsed;
		style.Colors[ImGuiCol_MenuBarBg]				= UIColor::MenuBarBg;

		style.Colors[ImGuiCol_ScrollbarBg]				= UIColor::ScrollbarBg;
		style.Colors[ImGuiCol_ScrollbarGrab]			= UIColor::ScrollbarGrab;
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= UIColor::ScrollbarGrabHovered;
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= UIColor::ScrollbarGrabActive;

		style.Colors[ImGuiCol_CheckMark]				= UIColor::Primary;
		style.Colors[ImGuiCol_SliderGrab]				= UIColor::Primary;
		style.Colors[ImGuiCol_SliderGrabActive]			= UIColor::PrimaryHover;

		style.Colors[ImGuiCol_Button]					= UIColor::Button;
		style.Colors[ImGuiCol_ButtonHovered]			= UIColor::ButtonHovered;
		style.Colors[ImGuiCol_ButtonActive]				= UIColor::ButtonActive;

		style.Colors[ImGuiCol_Header]					= UIColor::Header;					// Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		style.Colors[ImGuiCol_HeaderHovered]			= UIColor::HeaderHovered;
		style.Colors[ImGuiCol_HeaderActive]				= UIColor::HeaderActive;

		style.Colors[ImGuiCol_Separator]				= UIColor::Separator;
		style.Colors[ImGuiCol_SeparatorHovered]			= UIColor::SeparatorHovered;
		style.Colors[ImGuiCol_SeparatorActive]			= UIColor::SeparatorActive;

		style.Colors[ImGuiCol_ResizeGrip]				= UIColor::ResizeGrip;
		style.Colors[ImGuiCol_ResizeGripHovered]		= UIColor::ResizeGripHovered;
		style.Colors[ImGuiCol_ResizeGripActive]			= UIColor::ResizeGripActive;

		style.Colors[ImGuiCol_Tab]						= UIColor::Tab;
		style.Colors[ImGuiCol_TabHovered]				= UIColor::TabHovered;
		style.Colors[ImGuiCol_TabActive]				= UIColor::TabActive;
		style.Colors[ImGuiCol_TabUnfocused]				= UIColor::TabUnfocused;
		style.Colors[ImGuiCol_TabUnfocusedActive]		= UIColor::TabUnfocusedActive;

		style.Colors[ImGuiCol_DockingPreview]			= UIColor::DockingPreview;			// Preview overlay color when about to docking something
		style.Colors[ImGuiCol_DockingEmptyBg]			= UIColor::DockingEmptyBg;			// Background color for empty node (e.g. CentralNode with no window docked into it)

		style.Colors[ImGuiCol_PlotLines]				= UIColor::PlotLines;
		style.Colors[ImGuiCol_PlotLinesHovered]			= UIColor::PlotLinesHovered;
		style.Colors[ImGuiCol_PlotHistogram]			= UIColor::PlotHistogram;
		style.Colors[ImGuiCol_PlotHistogramHovered]		= UIColor::PlotHistogramHovered;

		style.Colors[ImGuiCol_TableHeaderBg]			= UIColor::TableHeaderBg;			// Table header background
		style.Colors[ImGuiCol_TableBorderStrong]		= UIColor::TableBorderStrong;		// Table outer and header borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableBorderLight]			= UIColor::TableBorderLight;			// Table inner borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableRowBg]				= UIColor::TableRowBg;				// Table row background (even rows)
		style.Colors[ImGuiCol_TableRowBgAlt]			= UIColor::TableRowBgAlt;			// Table row background (odd rows)
		
		style.Colors[ImGuiCol_TextSelectedBg]			= UIColor::TextSelectedBg;
		
		style.Colors[ImGuiCol_DragDropTarget]			= UIColor::DragDropTarget;
		style.Colors[ImGuiCol_NavHighlight]				= UIColor::NavHighlight;			// Gamepad/keyboard: current highlighted item

		style.Colors[ImGuiCol_NavWindowingHighlight]	= UIColor::NavWindowingHighlight;	// Highlight window when using CTRL+TAB
		style.Colors[ImGuiCol_NavWindowingDimBg]		= UIColor::NavWindowingDimBg;		// Darken/colorize entire screen behind the CTRL+TAB window list, when active
		style.Colors[ImGuiCol_ModalWindowDimBg]			= UIColor::ModalWindowDimBg;		// Darken/colorize entire screen behind a modal window, when one is active

		// Load default font
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;
		ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
		io.FontDefault = robotoFont;
	}

}
