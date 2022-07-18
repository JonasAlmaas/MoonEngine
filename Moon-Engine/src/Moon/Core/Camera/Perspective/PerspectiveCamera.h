#pragma once

#include "Moon/Core/Renderer/Camera/Perspective/PerspectiveRenderCamera.h"


namespace Moon {

	class PerspectiveCamera : public PerspectiveRenderCamera
	{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);

		virtual ~PerspectiveCamera() = default;
		
		void SetPosition(const glm::vec3& pos);
		inline glm::vec3 GetPosition() const { return m_Position; };
		inline glm::vec3& GetPosition() { return m_Position; };

		void SetPitchDegrees(float pitchDegrees);
		void SetPitchRadians(float pitchRadians);
		inline float GetPitchDegrees() const { return glm::degrees(m_Pitch); }
		inline float GetPitchRadians() const { return m_Pitch; }

		void SetYawDegrees(float yawDegrees);
		void SetYawRadians(float yawRadians);
		inline float GetYawDegrees() const { return glm::degrees(m_Yaw); }
		inline float GetYawRadians() const { return m_Yaw; }

		void SetRollDegrees(float rollDegrees);
		void SetRollRadians(float rollRadians);
		inline float GetRollDegrees() const { return glm::degrees(m_Roll); }
		inline float GetRollRadians() const { return m_Roll; }

		glm::vec3 GetUpVector() const;
		glm::vec3 GetRightVector() const;
		glm::vec3 GetForwardVector() const;

	protected:
		glm::quat GetOrientation() const;
		void UpdateView();

	protected:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		// Unit is radian
		float m_Pitch = glm::radians(-90.0f);
		float m_Yaw = 0.0f;
		float m_Roll = 0.0f;

	};

}
