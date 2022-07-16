#pragma once

#include "Moon/Core/Renderer/Camera/RenderCamera.h"


namespace Moon {

	class SceneCamera : public RenderCamera
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

		void SetViewWithTransform(const glm::mat4& transform) { m_View = glm::inverse(transform); }

		void SetViewportSize(uint32_t width, uint32_t height);

		void SetProjectionType(ProjectionType type);
		ProjectionType GetProjectionType() const { return m_ProjectionType; }

		void SetPerspectiveVerticalFOV(float verticalFov);
		float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }

		void SetPerspectiveNearClip(float nearClip);
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }

		void SetPerspectiveFarClip(float farClip);
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }

		void SetOrthographicSize(float size);
		float GetOrthographicSize() const { return m_OrthographicSize; }

		void SetOrthographicNearClip(float nearClip);
		float GetOrthographicNearClip() const { return m_OrthographicNear; }

		void SetOrthographicFarClip(float farClip);
		float GetOrthographicFarClip() const { return m_OrthographicFar; }

	private:
		void RecalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		float m_PerspectiveFOV = 45.0f;
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 10000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;

	};

}
