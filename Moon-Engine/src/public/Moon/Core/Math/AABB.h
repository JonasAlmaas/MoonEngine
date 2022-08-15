#pragma once

#include <glm/glm.hpp>


namespace Moon {

	// Axis Aligned Bounding Box
	struct AABB
	{
		glm::vec3 Min, Max;

		AABB();
		AABB(const glm::vec3& min, const glm::vec3& max);

		/**
		 * @return Whether p is within the aabb or not.
		*/
		bool IsWithin(const glm::vec3& p) const;
	};

	struct AABB2D
	{
		glm::vec2 Min, Max;

		AABB2D();
		AABB2D(const glm::vec2& min, const glm::vec2& max);

		/**
		 * @return Whether p is within the aabb or not.
		 */
		bool IsWithin(const glm::vec2& p) const;
	};

}
