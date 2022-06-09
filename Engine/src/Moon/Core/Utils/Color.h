#pragma once

#include <imgui.h>


namespace Moon {

	/** @enum Moon::ColorFormat
	 * @brief Helps with knowing how to tream values.
	 */
	enum class ColorFormat
	{
		None = 0,
		RGBADecimal,		/**< value between 0 -> 255 */
		RGBANormalized,		/**< value between 0 -> 1 */
	};

	struct Color
	{
		float r, g, b, a;
		ColorFormat Format;

		constexpr Color()
			: r(0.0f), g(0.0f), b(0.0f), a(1.0f), Format(ColorFormat::RGBANormalized) {}

		constexpr Color(float _v)
			: r(_v), g(_v), b(_v), a(1.0f), Format(ColorFormat::RGBANormalized) {}

		constexpr Color(ColorFormat _format, float _v)
			: r(_v), g(_v), b(_v), a(_format == ColorFormat::RGBADecimal ? 255.0f : 1.0f), Format(_format) {}

		constexpr Color(float _r, float _g, float _b)
			: r(_r), g(_g), b(_b), a(1.0f), Format(ColorFormat::RGBANormalized) {}

		constexpr Color(ColorFormat _format, float _r, float _g, float _b)
			: r(_r), g(_g), b(_b), a(_format == ColorFormat::RGBADecimal ? 255.0f : 1.0f), Format(_format) {}

		constexpr Color(float _v, float _a)
			: r(_v), g(_v), b(_v), a(_a), Format(ColorFormat::RGBANormalized) {}

		constexpr Color(ColorFormat _format, float _v, float _a)
			: r(_v), g(_v), b(_v), a(_a), Format(_format) {}

		constexpr Color(float _r, float _g, float _b, float _a)
			: r(_r), g(_g), b(_b), a(_a), Format(ColorFormat::RGBANormalized) {}

		constexpr Color(ColorFormat _format, float _r, float _g, float _b, float _a)
			: r(_r), g(_g), b(_b), a(_a), Format(_format) {}

		/**
		 * Sets the color format. This should only have to be used if Color was initialize Color without ColorFormat.
		 * 
		 * @param Format which is an enum from ColorFormat
		 */
		void SetColorFormat(ColorFormat _format) { Format = _format; }

		/**
		 * Converts an instance of the Color struct to an ImVec4 struct.
		 * 
		 * @return Color as an ImVec4.
		 */
		ImVec4 ToImVec4() const
		{
			switch (Format)
			{
				case ColorFormat::RGBADecimal:			return { r * DecimalToNormalizedStep, g * DecimalToNormalizedStep, b * DecimalToNormalizedStep, a * DecimalToNormalizedStep };
				case ColorFormat::RGBANormalized:		return { r, g, b, a };
			}
			return { r, g, b, a };
		}

	private:
		// I multiply by this instead of dividing by 255
		// to save a microscopical amount of time :)
		static const float DecimalToNormalizedStep;

	};
	const float Color::DecimalToNormalizedStep = 0.003921568627451f;


	class Colors
	{
	public:
		static const Color Primary;
		static const Color PrimaryHover;
		static const Color PrimaryPress;

		static const Color Text;
		static const Color TextDisabled;

		static const Color WindowBg;
		static const Color ChildBg;
		static const Color PopupBg;
		
		static const Color Border;

		static const Color ElementBg;
		static const Color ElementOutline;
		static const Color ElementHovered;
		static const Color ElementActive;
		
		static const Color TitleBg;
		static const Color TitleBgActive;
		static const Color TitleBgCollapsed;
		static const Color MenuBarBg;

		static const Color ScrollbarBg;
		static const Color ScrollbarGrab;
		static const Color ScrollbarGrabHovered;
		static const Color ScrollbarGrabActive;

		static const Color Button;
		static const Color ButtonHovered;
		static const Color ButtonActive;

		static const Color Header;
		static const Color HeaderHovered;
		static const Color HeaderActive;
		
		static const Color Separator;
		static const Color SeparatorHovered;
		static const Color SeparatorActive;

		static const Color ResizeGrip;
		static const Color ResizeGripHovered;
		static const Color ResizeGripActive;

		static const Color Tab;
		static const Color TabHovered;
		static const Color TabActive;
		static const Color TabUnfocused;
		static const Color TabUnfocusedActive;

		static const Color DockingPreview;
		static const Color DockingEmptyBg;

		static const Color PlotLines;
		static const Color PlotLinesHovered;
		static const Color PlotHistogram;
		static const Color PlotHistogramHovered;

		static const Color TableHeaderBg;
		static const Color TableBorderStrong;
		static const Color TableBorderLight;
		static const Color TableRowBg;
		static const Color TableRowBgAlt;

		static const Color TextSelectedBg;

		static const Color DragDropTarget;
		static const Color NavHighlight;

		static const Color NavWindowingHighlight;
		static const Color NavWindowingDimBg;
		static const Color ModalWindowDimBg;

	};

	const Color Colors::Primary					= { ColorFormat::RGBADecimal, 46, 104, 148 };
	const Color Colors::PrimaryHover			= { ColorFormat::RGBADecimal, 40, 85, 120 };
	const Color Colors::PrimaryPress			= { ColorFormat::RGBADecimal, 27, 59, 84 };

	const Color Colors::Text					= { ColorFormat::RGBADecimal, 242 };
	const Color Colors::TextDisabled			= { ColorFormat::RGBADecimal, 204 };

	const Color Colors::WindowBg				= { ColorFormat::RGBADecimal, 42 };
	const Color Colors::ChildBg					= { ColorFormat::RGBADecimal, 42 };
	const Color Colors::PopupBg					= { ColorFormat::RGBADecimal, 42 };
	
	const Color Colors::Border					= { ColorFormat::RGBADecimal, 56 };

	const Color Colors::ElementBg				= { ColorFormat::RGBADecimal, 20 };
	const Color Colors::ElementOutline			= { ColorFormat::RGBADecimal, 53 };
	const Color Colors::ElementHovered			= { ColorFormat::RGBADecimal, 26 };
	const Color Colors::ElementActive			= { ColorFormat::RGBADecimal, 26 };

	const Color Colors::TitleBg					= { ColorFormat::RGBADecimal, 26 };
	const Color Colors::TitleBgActive			= { ColorFormat::RGBADecimal, 26 };
	const Color Colors::TitleBgCollapsed		= { ColorFormat::RGBADecimal, 26 };
	const Color Colors::MenuBarBg				= { ColorFormat::RGBADecimal, 26 };

	const Color Colors::ScrollbarBg				= { ColorFormat::RGBADecimal, 36 };
	const Color Colors::ScrollbarGrab			= { ColorFormat::RGBADecimal, 109 };
	const Color Colors::ScrollbarGrabHovered	= { ColorFormat::RGBADecimal, 149 };
	const Color Colors::ScrollbarGrabActive		= { ColorFormat::RGBADecimal, 149 };

	const Color Colors::Button					= { ColorFormat::RGBADecimal, 65 };
	const Color Colors::ButtonHovered			= { ColorFormat::RGBADecimal, 110 };
	const Color Colors::ButtonActive			= { ColorFormat::RGBADecimal, 60 };

	const Color Colors::Header					= { ColorFormat::RGBADecimal, 65 };
	const Color Colors::HeaderHovered			= { ColorFormat::RGBADecimal, 65 };
	const Color Colors::HeaderActive			= { ColorFormat::RGBADecimal, 65 };

	const Color Colors::Separator				= { ColorFormat::RGBADecimal, 56 };
	const Color Colors::SeparatorHovered		= { ColorFormat::RGBADecimal, 56 };
	const Color Colors::SeparatorActive			= { ColorFormat::RGBADecimal, 56 };
	
	const Color Colors::ResizeGrip				= { ColorFormat::RGBADecimal, 65 };
	const Color Colors::ResizeGripHovered		= Colors::Primary;
	const Color Colors::ResizeGripActive		= Colors::Primary;

	const Color Colors::Tab						= Colors::WindowBg;
	const Color Colors::TabHovered				= { ColorFormat::RGBADecimal, 56 };
	const Color Colors::TabActive				= { ColorFormat::RGBADecimal, 56 };
	const Color Colors::TabUnfocused			= { ColorFormat::RGBADecimal, 56 };
	const Color Colors::TabUnfocusedActive		= { ColorFormat::RGBADecimal, 56 };

	const Color Colors::DockingPreview			= { ColorFormat::RGBADecimal, 44 };
	const Color Colors::DockingEmptyBg			= { ColorFormat::RGBADecimal, 48 };

	const Color Colors::PlotLines				= { ColorFormat::RGBADecimal, 156 };
	const Color Colors::PlotLinesHovered		= { ColorFormat::RGBADecimal, 255, 110, 89 };
	const Color Colors::PlotHistogram			= { ColorFormat::RGBADecimal, 230, 179, 0 };
	const Color Colors::PlotHistogramHovered	= { ColorFormat::RGBADecimal, 255, 153, 0 };

	const Color Colors::TableHeaderBg			= { ColorFormat::RGBADecimal, 25 };
	const Color Colors::TableBorderStrong		= { ColorFormat::RGBADecimal, 65 };
	const Color Colors::TableBorderLight		= { ColorFormat::RGBADecimal, 50 };
	const Color Colors::TableRowBg				= { ColorFormat::RGBADecimal, 0, 0 };
	const Color Colors::TableRowBgAlt			= { ColorFormat::RGBADecimal, 255, 8};

	const Color Colors::TextSelectedBg			= Colors::Primary;

	const Color Colors::DragDropTarget			= { ColorFormat::RGBADecimal, 85 };
	const Color Colors::NavHighlight			= { ColorFormat::RGBADecimal, 85 };

	const Color Colors::NavWindowingHighlight	= { ColorFormat::RGBADecimal, 85 };
	const Color Colors::NavWindowingDimBg		= { ColorFormat::RGBADecimal, 204, 51 };
	const Color Colors::ModalWindowDimBg		= { ColorFormat::RGBADecimal, 204, 89 };

}
