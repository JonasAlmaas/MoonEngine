#include "mepch.h"
#include "Moon/Core/Math/Spline.h"


namespace Moon {

	Spline::Spline(const std::vector<SplinePoint>& points)
		: m_Points(points), m_Looped(false)
	{
		CalculateTotalSplineLength();
	}

	Spline::Spline(const std::vector<SplinePoint>& points, bool looped)
		: m_Points(points), m_Looped(looped)
	{
		CalculateTotalSplineLength();
	}

	void Spline::SetLooped(bool looped)
	{
		m_Looped = looped;
		CalculateTotalSplineLength();
	}

	SplinePoint Spline::GetPoint(float t) const
	{
		int p0, p1, p2, p3;
		if (!m_Looped)
		{
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else
		{
			p1 = (int)t;
			p2 = (p1 + 1) % m_Points.size();
			p3 = (p2 + 1) % m_Points.size();
			p0 = p1 >= 1 ? p1 - 1 : (int)m_Points.size() - 1;
		}

		t = t - (int)t;

		float tt = t * t;
		float ttt = tt * t;

		float q0 = -ttt + 2.0f * tt - t;
		float q1 = 3.0f * ttt - 5.0f * tt + 2.0f;
		float q2 = -3.0f * ttt + 4.0f * tt + t;
		float q3 = ttt - tt;

		return (m_Points[p0] * q0 + m_Points[p1] * q1 + m_Points[p2] * q2 + m_Points[p3] * q3) * 0.5f;
	}

	glm::vec3 Spline::GetForwardVector(float t) const
	{
		int p0, p1, p2, p3;
		if (!m_Looped)
		{
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else
		{
			p1 = (int)t;
			p2 = (p1 + 1) % m_Points.size();
			p3 = (p2 + 1) % m_Points.size();
			p0 = p1 >= 1 ? p1 - 1 : (int)m_Points.size() - 1;
		}

		t = t - (int)t;

		float tt = t * t;
		float ttt = tt * t;

		float q0 = -3.0f * tt + 4.0f * t - 1;
		float q1 = 9.0f * tt - 10.0f * t;
		float q2 = -9.0f * tt + 8.0f * t + 1.0f;
		float q3 = 3.0f * tt - 2.0f * t;

		glm::vec3 forward = (m_Points[p0] * q0 + m_Points[p1] * q1 + m_Points[p2] * q2 + m_Points[p3] * q3) * 0.5f;
		return glm::normalize(forward);
	}

	float Spline::GetNormalisedOffset(float p) const
	{
		while (p > m_Length)
			p -= m_Length;

		int i = 0;
		while (p > m_Points[i].Length)
		{
			p -= m_Points[i].Length;
			i++;
		}

		return (float)i + (p / m_Points[i].Length);
	}

	float Spline::CalculateSegmentLength(uint32_t pointIndex)
	{
		float length = 0.0f;

		SplinePoint old_point, new_point;
		old_point = GetPoint((float)pointIndex);

		float stepSize = 0.005f;
		for (float t = stepSize; t <= 1.0f; t += stepSize)
		{
			new_point = GetPoint((float)pointIndex + t);
			length += sqrtf((new_point.x - old_point.x) * (new_point.x - old_point.x) + (new_point.y - old_point.y) * (new_point.y - old_point.y));
			old_point = new_point;
		}

		return length;
	}

	void Spline::CalculateTotalSplineLength()
	{
		m_Length = 0;

		for (uint32_t i = 0; i < m_Points.size(); i++)
		{
			m_Points[i].Length = CalculateSegmentLength(i);

			if (m_Looped)
			{
				m_Length += m_Points[i].Length;
			}
			else
			{
				if (i != 0 && i < GetRelevantPointCount())
					m_Length += m_Points[i].Length;
			}
		}
	}

}
