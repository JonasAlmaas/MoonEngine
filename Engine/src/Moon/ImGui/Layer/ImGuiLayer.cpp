#include "mepch.h"
#include "Moon/ImGui/Layer/ImGuiLayer.h"

#include "Moon/Core/Application/Application.h"
#include "Moon/Core/Customization/Color.h"

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

		//io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
		//io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f);

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

		style.Colors[ImGuiCol_Text]						= Colors::Text.ToImVec4();
		style.Colors[ImGuiCol_TextDisabled]				= Colors::TextDisabled.ToImVec4();
		
		style.Colors[ImGuiCol_WindowBg]					= Colors::WindowBg.ToImVec4();					// Background of normal windows
		style.Colors[ImGuiCol_ChildBg]					= Colors::ChildBg.ToImVec4();					// Background of child windows
		style.Colors[ImGuiCol_PopupBg]					= Colors::PopupBg.ToImVec4();					// Background of popups, menus, tooltips windows
		
		style.Colors[ImGuiCol_Border]					= Colors::Border.ToImVec4();
		style.Colors[ImGuiCol_BorderShadow]				= Colors::ElementOutline.ToImVec4();
		
		style.Colors[ImGuiCol_FrameBg]					= Colors::ElementBg.ToImVec4();					// Background of checkbox, radio button, plot, slider, text input
		style.Colors[ImGuiCol_FrameBgHovered]			= Colors::ElementHovered.ToImVec4();
		style.Colors[ImGuiCol_FrameBgActive]			= Colors::ElementActive.ToImVec4();
		
		style.Colors[ImGuiCol_TitleBg]					= Colors::TitleBg.ToImVec4();
		style.Colors[ImGuiCol_TitleBgActive]			= Colors::TitleBgActive.ToImVec4();
		style.Colors[ImGuiCol_TitleBgCollapsed]			= Colors::TitleBgCollapsed.ToImVec4();
		style.Colors[ImGuiCol_MenuBarBg]				= Colors::MenuBarBg.ToImVec4();

		style.Colors[ImGuiCol_ScrollbarBg]				= Colors::ScrollbarBg.ToImVec4();
		style.Colors[ImGuiCol_ScrollbarGrab]			= Colors::ScrollbarGrab.ToImVec4();
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= Colors::ScrollbarGrabHovered.ToImVec4();
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= Colors::ScrollbarGrabActive.ToImVec4();

		style.Colors[ImGuiCol_CheckMark]				= Colors::Primary.ToImVec4();
		style.Colors[ImGuiCol_SliderGrab]				= Colors::Primary.ToImVec4();
		style.Colors[ImGuiCol_SliderGrabActive]			= Colors::PrimaryHover.ToImVec4();

		style.Colors[ImGuiCol_Button]					= Colors::Button.ToImVec4();
		style.Colors[ImGuiCol_ButtonHovered]			= Colors::ButtonHovered.ToImVec4();
		style.Colors[ImGuiCol_ButtonActive]				= Colors::ButtonActive.ToImVec4();

		style.Colors[ImGuiCol_Header]					= Colors::Header.ToImVec4();					// Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		style.Colors[ImGuiCol_HeaderHovered]			= Colors::HeaderHovered.ToImVec4();
		style.Colors[ImGuiCol_HeaderActive]				= Colors::HeaderActive.ToImVec4();

		style.Colors[ImGuiCol_Separator]				= Colors::Separator.ToImVec4();
		style.Colors[ImGuiCol_SeparatorHovered]			= Colors::SeparatorHovered.ToImVec4();
		style.Colors[ImGuiCol_SeparatorActive]			= Colors::SeparatorActive.ToImVec4();

		style.Colors[ImGuiCol_ResizeGrip]				= Colors::ResizeGrip.ToImVec4();
		style.Colors[ImGuiCol_ResizeGripHovered]		= Colors::ResizeGripHovered.ToImVec4();
		style.Colors[ImGuiCol_ResizeGripActive]			= Colors::ResizeGripActive.ToImVec4();

		style.Colors[ImGuiCol_Tab]						= Colors::Tab.ToImVec4();
		style.Colors[ImGuiCol_TabHovered]				= Colors::TabHovered.ToImVec4();
		style.Colors[ImGuiCol_TabActive]				= Colors::TabActive.ToImVec4();
		style.Colors[ImGuiCol_TabUnfocused]				= Colors::TabUnfocused.ToImVec4();
		style.Colors[ImGuiCol_TabUnfocusedActive]		= Colors::TabUnfocusedActive.ToImVec4();

		style.Colors[ImGuiCol_DockingPreview]			= Colors::DockingPreview.ToImVec4();			// Preview overlay color when about to docking something
		style.Colors[ImGuiCol_DockingEmptyBg]			= Colors::DockingEmptyBg.ToImVec4();			// Background color for empty node (e.g. CentralNode with no window docked into it)

		style.Colors[ImGuiCol_PlotLines]				= Colors::PlotLines.ToImVec4();
		style.Colors[ImGuiCol_PlotLinesHovered]			= Colors::PlotLinesHovered.ToImVec4();
		style.Colors[ImGuiCol_PlotHistogram]			= Colors::PlotHistogram.ToImVec4();
		style.Colors[ImGuiCol_PlotHistogramHovered]		= Colors::PlotHistogramHovered.ToImVec4();

		style.Colors[ImGuiCol_TableHeaderBg]			= Colors::TableHeaderBg.ToImVec4();				// Table header background
		style.Colors[ImGuiCol_TableBorderStrong]		= Colors::TableBorderStrong.ToImVec4();			// Table outer and header borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableBorderLight]			= Colors::TableBorderLight.ToImVec4();			// Table inner borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableRowBg]				= Colors::TableRowBg.ToImVec4();				// Table row background (even rows)
		style.Colors[ImGuiCol_TableRowBgAlt]			= Colors::TableRowBgAlt.ToImVec4();				// Table row background (odd rows)
		
		style.Colors[ImGuiCol_TextSelectedBg]			= Colors::TextSelectedBg.ToImVec4();
		
		style.Colors[ImGuiCol_DragDropTarget]			= Colors::DragDropTarget.ToImVec4();
		style.Colors[ImGuiCol_NavHighlight]				= Colors::NavHighlight.ToImVec4();				// Gamepad/keyboard: current highlighted item

		style.Colors[ImGuiCol_NavWindowingHighlight]	= Colors::NavWindowingHighlight.ToImVec4();		// Highlight window when using CTRL+TAB
		style.Colors[ImGuiCol_NavWindowingDimBg]		= Colors::NavWindowingDimBg.ToImVec4();			// Darken/colorize entire screen behind the CTRL+TAB window list, when active
		style.Colors[ImGuiCol_ModalWindowDimBg]			= Colors::ModalWindowDimBg.ToImVec4();			// Darken/colorize entire screen behind a modal window, when one is active

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
