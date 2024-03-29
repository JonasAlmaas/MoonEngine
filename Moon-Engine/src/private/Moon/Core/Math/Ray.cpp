#include "mepch.h"
#include "Moon/Core/Math/Ray.h"


namespace Moon {

	bool Ray::IntersectsAABB(const AABB& aabb, float& t) const
	{
		// https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms

		glm::vec3 dirfrac = {
			1.0f / m_Direction.x,
			1.0f / m_Direction.y,
			1.0f / m_Direction.z
		};

		// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
		const glm::vec3& lb = aabb.Min;
		const glm::vec3& rt = aabb.Max;

		float t1 = (lb.x - m_Origin.x) * dirfrac.x;
		float t2 = (rt.x - m_Origin.x) * dirfrac.x;
		float t3 = (lb.y - m_Origin.y) * dirfrac.y;
		float t4 = (rt.y - m_Origin.y) * dirfrac.y;
		float t5 = (lb.z - m_Origin.z) * dirfrac.z;
		float t6 = (rt.z - m_Origin.z) * dirfrac.z;

		float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
		float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

		// If tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
		if (tmax < 0)
		{
			t = tmax;
			return false;
		}

		// If tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax)
		{
			t = tmax;
			return false;
		}

		t = tmin;
		return true;
	}

	bool Ray::IntersectsTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float& t) const
	{
		const glm::vec3 E1 = b - a;
		const glm::vec3 E2 = c - a;
		const glm::vec3 N = cross(E1, E2);
		const float det = -glm::dot(m_Direction, N);
		const float invdet = 1.0f / det;
		const glm::vec3 AO = m_Origin - a;
		const glm::vec3 DAO = glm::cross(AO, m_Direction);
		const float u = glm::dot(E2, DAO) * invdet;
		const float v = -glm::dot(E1, DAO) * invdet;
		t = glm::dot(AO, N) * invdet;
		return (det >= 1e-6f && t >= 0.0f && u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f);
	}

}
