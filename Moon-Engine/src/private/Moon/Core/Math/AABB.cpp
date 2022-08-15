#include "mepch.h"
#include "Moon/Core/Math/AABB.h"


namespace Moon {

	// -- AABB 3D --

	AABB::AABB()
		: Min(0.0f), Max(0.0f)
	{
	}

	AABB::AABB(const glm::vec3& min, const glm::vec3& max)
		: Min(min), Max(max)
	{
	}

	bool AABB::IsWithin(const glm::vec3& p) const
	{
		return p.x > Min.x && p.x < Max.x && p.y > Min.y && p.y < Max.y && p.z > Min.z && p.z < Max.z;
	}

	// -- AABB 2D --

	AABB2D::AABB2D()
		: Min(0.0f), Max(0.0f)
	{
	}

	AABB2D::AABB2D(const glm::vec2& min, const glm::vec2& max)
		: Min(min), Max(max)
	{
	}

	bool AABB2D::IsWithin(const glm::vec2& p) const
	{
		return p.x > Min.x && p.x < Max.x && p.y > Min.y && p.y < Max.y;
	}

}
