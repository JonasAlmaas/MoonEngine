#pragma once

#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicRenderCamera.h"


namespace Moon {

	class OrthographicCamera : public OrthographicRenderCamera
	{
	public:
		OrthographicCamera() = default;
		OrthographicCamera(float size, float aspectRatio);
		OrthographicCamera(float left, float right, float bottom, float top);

		virtual ~OrthographicCamera() = default;

		void SetPosition(const glm::vec3& pos);
		inline glm::vec3 GetPosition() { return m_Position; };
		inline glm::vec3* GetPositionValuePtr() { return &m_Position; }
		inline const glm::vec3* GetPositionValuePtr() const { return &m_Position; }

		void SetRotationDegrees(float rotationDegrees);
		void SetRotationRadians(float rotationRadians);
		inline const float GetRotationDegrees() const { return glm::degrees(m_Rotation); }
		inline const float GetRotationRadians() const { return m_Rotation; }

	protected:
		void UpdateView();

	protected:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		// Unit is radians
		float m_Rotation = 0.0f;

	};
}
