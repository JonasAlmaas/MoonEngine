#include "mepch.h"
#include "Moon/ImGui/ImGuiLayer.h"

#include "Moon/Core/Application/Application.h"
#include "Moon/Core/Customization/UIColor.h"
#include "Moon/Core/Type/Color.h"

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

	ImGuiLayer::~ImGuiLayer()
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
		style.WindowBorderSize = 0.0f;
		style.WindowMinSize = ImVec2(10.0f, 10.0f);
		style.WindowTitleAlign = ImVec2(0, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.PopupRounding = 4.0f;
		style.PopupBorderSize = 0.0f;
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

		style.Colors[ImGuiCol_Text]						= UIColor::Text.ToImVec4();
		style.Colors[ImGuiCol_TextDisabled]				= UIColor::TextDisabled.ToImVec4();
		
		style.Colors[ImGuiCol_WindowBg]					= UIColor::WindowBg.ToImVec4();					// Background of normal windows
		style.Colors[ImGuiCol_ChildBg]					= UIColor::ChildBg.ToImVec4();					// Background of child windows
		style.Colors[ImGuiCol_PopupBg]					= UIColor::PopupBg.ToImVec4();					// Background of popups, menus, tooltips windows
		
		style.Colors[ImGuiCol_Border]					= UIColor::Border.ToImVec4();
		style.Colors[ImGuiCol_BorderShadow]				= UIColor::ElementOutline.ToImVec4();
		
		style.Colors[ImGuiCol_FrameBg]					= UIColor::ElementBg.ToImVec4();				// Background of checkbox, radio button, plot, slider, text input
		style.Colors[ImGuiCol_FrameBgHovered]			= UIColor::ElementHovered.ToImVec4();
		style.Colors[ImGuiCol_FrameBgActive]			= UIColor::ElementActive.ToImVec4();
		
		style.Colors[ImGuiCol_TitleBg]					= UIColor::TitleBg.ToImVec4();
		style.Colors[ImGuiCol_TitleBgActive]			= UIColor::TitleBgActive.ToImVec4();
		style.Colors[ImGuiCol_TitleBgCollapsed]			= UIColor::TitleBgCollapsed.ToImVec4();
		style.Colors[ImGuiCol_MenuBarBg]				= UIColor::MenuBarBg.ToImVec4();

		style.Colors[ImGuiCol_ScrollbarBg]				= UIColor::ScrollbarBg.ToImVec4();
		style.Colors[ImGuiCol_ScrollbarGrab]			= UIColor::ScrollbarGrab.ToImVec4();
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= UIColor::ScrollbarGrabHovered.ToImVec4();
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= UIColor::ScrollbarGrabActive.ToImVec4();

		style.Colors[ImGuiCol_CheckMark]				= UIColor::Primary.ToImVec4();
		style.Colors[ImGuiCol_SliderGrab]				= UIColor::Primary.ToImVec4();
		style.Colors[ImGuiCol_SliderGrabActive]			= UIColor::PrimaryHover.ToImVec4();

		style.Colors[ImGuiCol_Button]					= UIColor::Button.ToImVec4();
		style.Colors[ImGuiCol_ButtonHovered]			= UIColor::ButtonHovered.ToImVec4();
		style.Colors[ImGuiCol_ButtonActive]				= UIColor::ButtonActive.ToImVec4();

		style.Colors[ImGuiCol_Header]					= UIColor::Header.ToImVec4();					// Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		style.Colors[ImGuiCol_HeaderHovered]			= UIColor::HeaderHovered.ToImVec4();
		style.Colors[ImGuiCol_HeaderActive]				= UIColor::HeaderActive.ToImVec4();

		style.Colors[ImGuiCol_Separator]				= UIColor::Separator.ToImVec4();
		style.Colors[ImGuiCol_SeparatorHovered]			= UIColor::SeparatorHovered.ToImVec4();
		style.Colors[ImGuiCol_SeparatorActive]			= UIColor::SeparatorActive.ToImVec4();

		style.Colors[ImGuiCol_ResizeGrip]				= UIColor::ResizeGrip.ToImVec4();
		style.Colors[ImGuiCol_ResizeGripHovered]		= UIColor::ResizeGripHovered.ToImVec4();
		style.Colors[ImGuiCol_ResizeGripActive]			= UIColor::ResizeGripActive.ToImVec4();

		style.Colors[ImGuiCol_Tab]						= UIColor::Tab.ToImVec4();
		style.Colors[ImGuiCol_TabHovered]				= UIColor::TabHovered.ToImVec4();
		style.Colors[ImGuiCol_TabActive]				= UIColor::TabActive.ToImVec4();
		style.Colors[ImGuiCol_TabUnfocused]				= UIColor::TabUnfocused.ToImVec4();
		style.Colors[ImGuiCol_TabUnfocusedActive]		= UIColor::TabUnfocusedActive.ToImVec4();

		style.Colors[ImGuiCol_DockingPreview]			= UIColor::DockingPreview.ToImVec4();			// Preview overlay color when about to docking something
		style.Colors[ImGuiCol_DockingEmptyBg]			= UIColor::DockingEmptyBg.ToImVec4();			// Background color for empty node (e.g. CentralNode with no window docked into it)

		style.Colors[ImGuiCol_PlotLines]				= UIColor::PlotLines.ToImVec4();
		style.Colors[ImGuiCol_PlotLinesHovered]			= UIColor::PlotLinesHovered.ToImVec4();
		style.Colors[ImGuiCol_PlotHistogram]			= UIColor::PlotHistogram.ToImVec4();
		style.Colors[ImGuiCol_PlotHistogramHovered]		= UIColor::PlotHistogramHovered.ToImVec4();

		style.Colors[ImGuiCol_TableHeaderBg]			= UIColor::TableHeaderBg.ToImVec4();			// Table header background
		style.Colors[ImGuiCol_TableBorderStrong]		= UIColor::TableBorderStrong.ToImVec4();		// Table outer and header borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableBorderLight]			= UIColor::TableBorderLight.ToImVec4();			// Table inner borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableRowBg]				= UIColor::TableRowBg.ToImVec4();				// Table row background (even rows)
		style.Colors[ImGuiCol_TableRowBgAlt]			= UIColor::TableRowBgAlt.ToImVec4();			// Table row background (odd rows)
		
		style.Colors[ImGuiCol_TextSelectedBg]			= UIColor::TextSelectedBg.ToImVec4();
		
		style.Colors[ImGuiCol_DragDropTarget]			= UIColor::DragDropTarget.ToImVec4();
		style.Colors[ImGuiCol_NavHighlight]				= UIColor::NavHighlight.ToImVec4();				// Gamepad/keyboard: current highlighted item

		style.Colors[ImGuiCol_NavWindowingHighlight]	= UIColor::NavWindowingHighlight.ToImVec4();	// Highlight window when using CTRL+TAB
		style.Colors[ImGuiCol_NavWindowingDimBg]		= UIColor::NavWindowingDimBg.ToImVec4();		// Darken/colorize entire screen behind the CTRL+TAB window list, when active
		style.Colors[ImGuiCol_ModalWindowDimBg]			= UIColor::ModalWindowDimBg.ToImVec4();			// Darken/colorize entire screen behind a modal window, when one is active

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowBorderSize = 0.0f;
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

}
