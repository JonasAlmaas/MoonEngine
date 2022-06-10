#pragma once

#include <imgui.h>


namespace Moon {

	/**
	 * @brief Helps with knowing how to tream values.
	 */
	enum class ColorFormat
	{
		None = 0,
		RGBADecimal,		/**< range: 0 to 255 */
		RGBANormalized,		/**< range: 0 to 1 */
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
		 * @return A new instance of Color.
		 */
		Color Copy() const
		{
			return Color(Format, r, g, b, a);
		}

		/**
		 * Sets the color format. This should only have to be used if Color was initialize Color without ColorFormat.
		 * 
		 * @param Format which is an enum from ColorFormat
		 */
		void SetColorFormat(ColorFormat _format)
		{
			Format = _format;
		}

		/**
		 * @brief Converts the color into a 0 to 1 range.
		 * This only happends if it is not already normalized.
		 */
		void Normalize()
		{
			switch (Format)
			{
				case ColorFormat::RGBADecimal:
				{
					r = r * DecimalToNormalizedStep;
					g = g * DecimalToNormalizedStep;
					b = b * DecimalToNormalizedStep;
					a = a * DecimalToNormalizedStep;
				}
			}
		}

		/**
		 * @return A normalized copy of the Color.
		*/
		Color GetNormalized() const
		{
			Color c = this->Copy();
			c.Normalize();
			return c;
		}

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
			}
			return { r, g, b, a };
		}

	private:
		// I multiply by this instead of dividing by 255
		// to save a microscopical amount of time :)
		static constexpr float DecimalToNormalizedStep = 0.003921568627451f;

	};

}
