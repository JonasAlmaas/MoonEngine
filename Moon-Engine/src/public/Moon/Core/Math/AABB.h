#pragma once

#include <glm/glm.hpp>


namespace Moon {

	// Axis Aligned Bounding Box
	struct AABB
	{
		glm::vec3 Min, Max;

		AABB()
			: Min(0.0f), Max(0.0f) {}

		AABB(const glm::vec3& min, const glm::vec3& max)
			: Min(min), Max(max) {}
	};

	struct AABB2D
	{
		glm::vec2 Min, Max;

		AABB2D()
			: Min(0.0f), Max(0.0f) {}

		AABB2D(const glm::vec2& min, const glm::vec2& max)
			: Min(min), Max(max) {}
	};

}
