#pragma once


namespace Moon {

	class Math
	{
	public:
		/**
		 * @return x + ((y - x) * a), i.e., the linear blend of x and y using the floating-point value a.
		 * The value for a is not restricted to the range [0, 1].
		 */
		static float Lerp(float x, float y, float a)
		{
			return x + ((y - x) * a);
		}

		static float Length(glm::vec3 vec)
		{
			return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		}

		static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

	};

}
