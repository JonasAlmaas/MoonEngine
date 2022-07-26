#pragma once

#include "Moon/Core/Math.h"
#include "Moon/Core/Util/Conversion/Hexadecimal.h"


namespace Moon {

	class Color
	{
	public:
		/**
		 * @brief RGBA.
		 * Default color is full white.
		 */
		Color()
			: r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}


		/**
		 * Grayscale float.
		 *
		 * @param Scalar value used for color rgb.
		 */
		Color(float s)
			: r(s), g(s), b(s), a(1.0f) {}

		/**
		 * Grayscale float.
		 *
		 * @param Scalar value used for color rgb.
		 * @param Alpha for color alpha.
		 */
		Color(float s, float a)
			: r(s), g(s), b(s), a(a) {}

		/**
		 * RGB float.
		 *
		 * @param rgb, usaly in the range 0 to 1. (But can be higher.)
		 */
		Color(float r, float g, float b)
			: r(r), g(g), b(b), a(1.0f) {}

		/**
		 * RGBA float.
		 *
		 * @param rgba, usaly in the range 0 to 1. (But can be higher.)
		 */
		Color(float r, float g, float b, float a)
			: r(r), g(g), b(b), a(a) {}

		/**
		 * Grayscale double (stored as float).
		 *
		 * @param Scalar value used for color rgb.
		 */
		Color(double s)
			: r((float)s), g((float)s), b((float)s), a(1.0f) {}

		/**
		 * Grayscale double (stored as float).
		 *
		 * @param Scalar value used for color rgb.
		 * @param Alpha for color alpha.
		 */
		Color(double s, double a)
			: r((float)s), g((float)s), b((float)s), a((float)a) {}

		/**
		 * RGB double (stored as float).
		 *
		 * @param rgb, usaly in the range 0 to 1. (But can be higher.)
		 */
		Color(double r, double g, double b)
			: r((float)r), g((float)g), b((float)b), a(1.0f) {}

		/**
		 * RGBA double (stored as float).
		 *
		 * @param rgba, usaly in the range 0 to 1. (But can be higher.)
		 */
		Color(double r, double g, double b, double a)
			: r((float)r), g((float)g), b((float)b), a((float)a) {}

		/**
		 * Grayscale decimal (stored as float).
		 *
		 * Value usaly ranges from 0 to 255. (But can be higher.)
		 * @param Scalar value used for color rgb.
		 */
		Color(int s)
			: r((float)s* DecimalConversionFactor), g((float)s* DecimalConversionFactor), b((float)s* DecimalConversionFactor), a(1.0f) {}

		/**
		 * Grayscale decimal (stored as float).
		 *
		 * Value usaly ranges from 0 to 255. (But can be higher.)
		 * @param Scalar value used for color rgb.
		 * @param Alpha for color alpha.
		 */
		Color(int s, int a)
			: r((float)s* DecimalConversionFactor), g((float)s* DecimalConversionFactor), b((float)s* DecimalConversionFactor), a((float)a* DecimalConversionFactor) {}

		/**
		 * RGB decimal (stored as float).
		 *
		 * @param rgb, usaly in the range 0 to 255. (But can be higher.)
		 */
		Color(int r, int g, int b)
			: r((float)r* DecimalConversionFactor), g((float)g* DecimalConversionFactor), b((float)b* DecimalConversionFactor), a(1.0f) {}

		/**
		 * RGBA decimal (stored as float).
		 *
		 * @param rgba, usaly in the range 0 to 255. (But can be higher.)
		 */
		Color(int r, int g, int b, int a)
			: r((float)r* DecimalConversionFactor), g((float)g* DecimalConversionFactor), b((float)b* DecimalConversionFactor), a((float)a* DecimalConversionFactor) {}

		/**
		 * RGBA decimal (stored as float).
		 *
		 * @param rgba as a string
		 */
		Color(const std::string& hexValue)
			: r(1.0f), g(1.0f), b(1.0f), a(1.0f)
		{
			ME_CORE_ASSERT(hexValue[0] == '#', "Incorrect declaration of a hex value!");

			switch (hexValue.length())
			{
				// Grayscale ("#ff")
				case 3:
				{
					r = (float)Hex::HexToByte({ hexValue[1], hexValue[2], 0 }) * DecimalConversionFactor;
					g = r;
					b = r;
					a = 1.0f;
					break;
				}
				// Grayscale Alpha ("#ff00")
				case 5:
				{
					r = (float)Hex::HexToByte({ hexValue[1], hexValue[2], 0 }) * DecimalConversionFactor;
					g = r;
					b = r;
					a = (float)Hex::HexToByte({ hexValue[7], hexValue[8], 0 }) * DecimalConversionFactor;
					break;
				}
				// RGB ("#ff00ff")
				case 7:
				{
					r = (float)Hex::HexToByte({ hexValue[1], hexValue[2], 0 }) * DecimalConversionFactor;
					g = (float)Hex::HexToByte({ hexValue[3], hexValue[4], 0 }) * DecimalConversionFactor;
					b = (float)Hex::HexToByte({ hexValue[5], hexValue[6], 0 }) * DecimalConversionFactor;
					a = 1.0f;
					break;
				}
				// RGBA ("#ff00ff00")
				case 9:
				{
					r = (float)Hex::HexToByte({ hexValue[1], hexValue[2], 0 }) * DecimalConversionFactor;
					g = (float)Hex::HexToByte({ hexValue[3], hexValue[4], 0 }) * DecimalConversionFactor;
					b = (float)Hex::HexToByte({ hexValue[5], hexValue[6], 0 }) * DecimalConversionFactor;
					a = (float)Hex::HexToByte({ hexValue[7], hexValue[8], 0 }) * DecimalConversionFactor;
					break;
				}
			}
		}

		Color(const Color& other)
			: r(other.r), g(other.g), b(other.b), a(other.a) {}

	public:
		/**
		 * Gives an interpolated new color based on a factor.
		 *
		 * @param Color you want to interpolate with.
		 * @param interpolation factor.
		 */
		Color Lerp(const Color& other, float factor)
		{
			return {
				Math::Lerp(r, other.r, factor),
				Math::Lerp(g, other.g, factor),
				Math::Lerp(b, other.b, factor),
				Math::Lerp(a, other.a, factor)
			};
		}

	public:
		bool operator==(const Color& other)
		{
			return (r == other.r) && (g == other.g) && (b == other.b) && (a == other.a);
		}

		bool operator!=(const Color& other)
		{
			return !(*this == other);
		}

		// -- Type override --

		operator float* ()
		{
			return &r;
		}

		operator glm::vec3()
		{
			return { r, g, b };
		}

		operator const glm::vec3() const
		{
			return { r, g, b };
		}

		operator glm::vec4()
		{
			return { r, g, b, a };
		}

		operator const glm::vec4() const
		{
			return { r, g, b, a };
		}

		operator ImVec4() const
		{
			return { r, g, b, a };
		}

		operator std::string() const
		{
			return "Color: (r: " + std::to_string(r) + ", g: " + std::to_string(g) + ", b: " + std::to_string(b) + ", a: " + std::to_string(a) + ")";
		}

	public:
		float r, g, b, a;

	private:
		static constexpr float DecimalConversionFactor = 0.003921568627451f;

	};

}
