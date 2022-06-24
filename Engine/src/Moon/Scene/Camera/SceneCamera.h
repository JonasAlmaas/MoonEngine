#pragma once

#include "Moon/Core/Renderer/Camera/Camera.h"


namespace Moon {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic = 1,
		};

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetPerspective(float verticalFov, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type);

		/**
		 * @return Cameras vertical FOV in degrees.
		 */
		float GetPerspectiveVerticalFOV() const { return glm::degrees(m_PerspectiveFOV); }

		/**
		 * Sets the vertical field of view for the camera.
		 * 
		 * @param Vertical FOV in degrees.
		 */
		void SetPerspectiveVerticalFOV(float verticalFov);

		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip);

		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size);

		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float nearClip);

		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float farClip);

	private:
		void RecalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 10000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;

	};

}
