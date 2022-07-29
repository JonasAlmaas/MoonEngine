#pragma once

#include "Moon/Core/Math/AABB.h"


namespace Moon {

	class Ray
	{
	public:
		Ray(const glm::vec3& origin, const glm::vec3& direction)
			: m_Origin(origin), m_Direction(direction) {}

		void SetOrigin(const glm::vec3& origin) { m_Origin = origin; }
		glm::vec3 GetOrigin() const { return m_Origin; }

		void SetDirection(const glm::vec3& direction) { m_Direction = direction; }
		glm::vec3 GetDirection() const { return m_Direction; }

		/**
		 * @param Axis Aligned Bounding Box (AABB).
		 * @param t distance to intersection.
		 *
		 * @return Whether the ray hit or not.
		 */
		bool IntersectsAABB(const AABB& aabb, float& t) const;

		/**
		 * @param 3 positions forming a triangle.
		 * @param t distance to intersection.
		 * 
		 * @return Whether the ray hit or not.
		 */
		bool IntersectsTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float& t) const;

	public:
		static Ray Zero()
		{
			return { {0.0f, 0.0f, 0.0f},{0.0f, 0.0f, 0.0f} };
		}

	private:
		glm::vec3 m_Origin, m_Direction;

	};

}
