#pragma once

#include "Moon/Core/Type/Color.h"


namespace Moon {

	/**
	 * @brief A pure static class that stores colors used for UI.
	 */
	class UIColor
	{
	public:
		static constexpr Color Primary = { ColorFormat::RGBADecimal, 46, 104, 148 };
		static constexpr Color PrimaryHover = { ColorFormat::RGBADecimal, 40, 85, 120 };
		static constexpr Color PrimaryPress = { ColorFormat::RGBADecimal, 27, 59, 84 };

		static constexpr Color Text = { ColorFormat::RGBADecimal, 242 };
		static constexpr Color TextDisabled = { ColorFormat::RGBADecimal, 204 };

		static constexpr Color WindowBg = { ColorFormat::RGBADecimal, 42 };
		static constexpr Color ChildBg = { ColorFormat::RGBADecimal, 42 };
		static constexpr Color PopupBg = { ColorFormat::RGBADecimal, 42 };

		static constexpr Color Border = { ColorFormat::RGBADecimal, 56 };

		static constexpr Color ElementBg = { ColorFormat::RGBADecimal, 20 };
		static constexpr Color ElementOutline = { ColorFormat::RGBADecimal, 53 };
		static constexpr Color ElementHovered = { ColorFormat::RGBADecimal, 26 };
		static constexpr Color ElementActive = { ColorFormat::RGBADecimal, 26 };

		static constexpr Color TitleBg = { ColorFormat::RGBADecimal, 26 };
		static constexpr Color TitleBgActive = { ColorFormat::RGBADecimal, 26 };
		static constexpr Color TitleBgCollapsed = { ColorFormat::RGBADecimal, 26 };
		static constexpr Color MenuBarBg = { ColorFormat::RGBADecimal, 26 };

		static constexpr Color ScrollbarBg = { ColorFormat::RGBADecimal, 36 };
		static constexpr Color ScrollbarGrab = { ColorFormat::RGBADecimal, 109 };
		static constexpr Color ScrollbarGrabHovered = { ColorFormat::RGBADecimal, 149 };
		static constexpr Color ScrollbarGrabActive = { ColorFormat::RGBADecimal, 149 };

		static constexpr Color Button = { ColorFormat::RGBADecimal, 65 };
		static constexpr Color ButtonHovered = { ColorFormat::RGBADecimal, 110 };
		static constexpr Color ButtonActive = { ColorFormat::RGBADecimal, 60 };

		static constexpr Color Header = { ColorFormat::RGBADecimal, 65 };
		static constexpr Color HeaderHovered = { ColorFormat::RGBADecimal, 65 };
		static constexpr Color HeaderActive = { ColorFormat::RGBADecimal, 65 };

		static constexpr Color Separator = { ColorFormat::RGBADecimal, 56 };
		static constexpr Color SeparatorHovered = { ColorFormat::RGBADecimal, 56 };
		static constexpr Color SeparatorActive = { ColorFormat::RGBADecimal, 56 };

		static constexpr Color ResizeGrip = { ColorFormat::RGBADecimal, 65 };
		static constexpr Color ResizeGripHovered = Primary;
		static constexpr Color ResizeGripActive = Primary;

		static constexpr Color Tab = WindowBg;
		static constexpr Color TabHovered = { ColorFormat::RGBADecimal, 56 };
		static constexpr Color TabActive = { ColorFormat::RGBADecimal, 56 };
		static constexpr Color TabUnfocused = { ColorFormat::RGBADecimal, 56 };
		static constexpr Color TabUnfocusedActive = { ColorFormat::RGBADecimal, 56 };

		static constexpr Color DockingPreview = { ColorFormat::RGBADecimal, 44 };
		static constexpr Color DockingEmptyBg = { ColorFormat::RGBADecimal, 48 };

		static constexpr Color PlotLines = { ColorFormat::RGBADecimal, 156 };
		static constexpr Color PlotLinesHovered = { ColorFormat::RGBADecimal, 255, 110, 89 };
		static constexpr Color PlotHistogram = { ColorFormat::RGBADecimal, 230, 179, 0 };
		static constexpr Color PlotHistogramHovered = { ColorFormat::RGBADecimal, 255, 153, 0 };

		static constexpr Color TableHeaderBg = { ColorFormat::RGBADecimal, 25 };
		static constexpr Color TableBorderStrong = { ColorFormat::RGBADecimal, 65 };
		static constexpr Color TableBorderLight = { ColorFormat::RGBADecimal, 50 };
		static constexpr Color TableRowBg = { ColorFormat::RGBADecimal, 0, 0 };
		static constexpr Color TableRowBgAlt = { ColorFormat::RGBADecimal, 255, 8 };

		static constexpr Color TextSelectedBg = Primary;

		static constexpr Color DragDropTarget = { ColorFormat::RGBADecimal, 85 };
		static constexpr Color NavHighlight = { ColorFormat::RGBADecimal, 85 };

		static constexpr Color NavWindowingHighlight = { ColorFormat::RGBADecimal, 85 };
		static constexpr Color NavWindowingDimBg = { ColorFormat::RGBADecimal, 204, 51 };
		static constexpr Color ModalWindowDimBg = { ColorFormat::RGBADecimal, 204, 89 };

	};

}