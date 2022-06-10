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


}
