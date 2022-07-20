#pragma once

#include "Moon/Core/Renderer/Camera/RenderCamera.h"


namespace Moon {

	class PerspectiveRenderCamera : public RenderCamera
	{
	public:
		PerspectiveRenderCamera() = default;
		PerspectiveRenderCamera(float fov, float aspectRatio, float nearClip, float farClip);

		virtual ~PerspectiveRenderCamera() = default;

		void SetViewportSize(float width, float height);
		inline float GetViewportWidth() const { return m_ViewportWidth; }
		inline float GetViewportHeight() const { return m_ViewportHeight; }

		void SetFOV(float fov);
		inline float GetFOV() const { return m_FOV; }

		void SetAspectRatio(float aspectRatio);
		inline float GetAspectRatio() const { return m_AspectRatio; }

		void SetNearClip(float nearClip);
		inline float GetNearClip() const { return m_NearClip; }

		void SetFarClip(float farClip);
		inline float GetFarClip() const { return m_FarClip; }

	protected:
		void UpdateProjection();

	protected:
		float m_FOV = 45.0f;
		float m_AspectRatio = 1.778f;
		float m_NearClip = 0.1f;
		float m_FarClip = 10000.0f;

		float m_ViewportWidth = 1920.0f;
		float m_ViewportHeight = 1080.0f;

	};

}
