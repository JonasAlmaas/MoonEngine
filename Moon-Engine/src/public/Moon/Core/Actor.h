#pragma once


namespace Moon {

	class Actor
	{
	public:
		Actor() = default;
		virtual ~Actor() = default;

		// -- Positions --
		virtual void SetPosition(const glm::vec3& pos) { m_Position = pos; }
		inline glm::vec3 GetPosition() const { return m_Position; }
		inline glm::vec3& GetPosition() { return m_Position; }

		// -- Rotaion --
		virtual void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
		inline glm::vec3 GetRotation() const { return m_Rotation; }
		inline glm::vec3& GetRotation() { return m_Rotation; }

		// -- Pitch --
		virtual void SetPitchDegrees(float pitchDegrees) { m_Pitch = glm::radians(pitchDegrees); }
		virtual void SetPitchRadians(float pitchRadians) { m_Pitch = pitchRadians; }
		inline float GetPitchDegrees() const { return glm::degrees(m_Pitch); }
		inline float GetPitchRadians() const { return m_Pitch; }

		// -- Yaw --
		virtual void SetYawDegrees(float yawDegrees) { m_Yaw = glm::radians(yawDegrees); }
		virtual void SetYawRadians(float yawRadians) { m_Yaw = yawRadians; }
		inline float GetYawDegrees() const { return glm::degrees(m_Yaw); }
		inline float GetYawRadians() const { return m_Yaw; }

		// --- Roll --
		virtual void SetRollDegrees(float rollDegrees) { m_Roll = glm::radians(rollDegrees); }
		virtual void SetRollRadians(float rollRadians) { m_Roll = rollRadians; }
		inline float GetRollDegrees() const { return glm::degrees(m_Roll); }
		inline float GetRollRadians() const { return m_Roll; }

		// -- Scale --
		virtual void SetScale(float scale)
		{
			m_Scale.x = scale;
			m_Scale.y = scale;
			m_Scale.z = scale;
		}

		virtual void SetScale(const glm::vec3& scale) { m_Scale = scale; }
		inline glm::vec3 GetScale() const { return m_Scale; }

		virtual void SetTransform(const glm::mat4& transform);
		glm::mat4 GetTransform() const;

		glm::vec3 GetUpVector() const;
		glm::vec3 GetRightVector() const;
		glm::vec3 GetForwardVector() const;

	protected:
		glm::quat GetOrientation() const;

	protected:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		union
		{
			glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f }; // Unit is radians
			struct { float m_Pitch, m_Yaw, m_Roll; };
		};

		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };;

	};

}
