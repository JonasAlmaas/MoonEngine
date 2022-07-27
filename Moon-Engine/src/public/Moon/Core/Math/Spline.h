#pragma once


namespace Moon {

	struct SplinePoint
	{
		float x = 0.0f, y = 0.0f, z = 0.0f;
		float Length;

		operator glm::vec3() { return { x, y, z };}
		operator glm::vec3() const { return { x, y, z }; }

		SplinePoint operator+(const glm::vec3& other) const { return { x + other.x, y + other.y, z + other.z }; }
		SplinePoint operator-(const glm::vec3& other) const { return { x - other.x, y - other.y, z - other.z }; }

		SplinePoint operator*(float factor) const { return { x * factor, y * factor, z * factor }; }
		SplinePoint operator/(float divisor) const { return { x / divisor, y / divisor, z / divisor }; }
	};

	/**
	 * Catmull-Rom Spline
	 * https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline
	 */
	class Spline
	{
	public:
		Spline(const std::vector<SplinePoint>& points);
		Spline(const std::vector<SplinePoint>& points, bool looped);

		void SetLooped(bool looped);
		inline bool GetLooped() const { return m_Looped; }

		/**
		 * @param Fractional value t.
		 * t = 1.5f would be between the second and third spline point.
		 * 
		 * @return SplinePoint based on t and whether it is looped or not.
		 */
		SplinePoint GetPoint(float t) const;

		glm::vec3 GetForwardVector(float t) const;

		/**
		 * @param World distance along the spline.
		 * @return Fractional value to be used as input for GetPoint(...) or GetForwardVector(...)
		 */
		float GetNormalisedOffset(float p) const;

		/**
		 * @return Total length of the spline.
		 */
		inline float GetLength() const { return m_Length; }

		/**
		 * Returns the amount of points. Excluding ends if m_Looped if set to false.
		 */
		uint32_t GetRelevantPointCount() const { return m_Looped ? (uint32_t)m_Points.size() : (uint32_t)m_Points.size() - 3U; }

	public:
		std::vector<SplinePoint>::iterator begin() { return m_Points.begin(); }
		std::vector<SplinePoint>::iterator end() { return m_Points.end(); }

	private:
		float CalculateSegmentLength(uint32_t pointIndex);
		void CalculateTotalSplineLength();

	private:
		std::vector<SplinePoint> m_Points;
		bool m_Looped;
		float m_Length;

	};

}
