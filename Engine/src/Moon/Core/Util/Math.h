#pragma once


namespace Moon {

	class Math
	{
	public:
		/**
		 * @brief Returns x + ((y - x) * a), i.e., the linear blend of x and y using the floating-point value a.
		 * The value for a is not restricted to the range [0, 1].
		 */
		static float Lerp(float x, float y, float a)
		{
			return x + ((y - x) * a);
		}

	};

}
