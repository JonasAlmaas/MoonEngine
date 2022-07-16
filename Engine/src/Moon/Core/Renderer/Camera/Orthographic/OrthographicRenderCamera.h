#pragma once

#include "Moon/Core/Renderer/Camera/RenderCamera.h"


namespace Moon {

	class OrthographicRenderCamera : public RenderCamera
	{
	public:
		OrthographicRenderCamera() = default;
		OrthographicRenderCamera(float size, float aspectRatio);
		OrthographicRenderCamera(float left, float right, float bottom, float top);

		virtual ~OrthographicRenderCamera() = default;

		void SetSize(float size);
		inline float GetSize() const { return m_Size; }

		void SetBounds(float left, float right, float bottom, float top);
		float GetLeft() const { return m_Left; }
		float GetRight() const { return m_Right; }
		float GetBottom() const { return m_Bottom; }
		float GetTop() const { return m_Top; }

		void SetViewportSize(float width, float height);
		inline float GetViewportWidth() const { return m_ViewportWidth; }
		inline float GetViewportHeight() const { return m_ViewportHeight; }

		void SetAspectRatio(float aspectRatio);
		inline float GetAspectRatio() const { return m_AspectRatio; }

		void SetNearClip(float nearClip);
		inline float GetNearClip() const { return m_NearClip; }

		void SetFarClip(float farClip);
		inline float GetFarClip() const { return m_FarClip; }

	protected:
		void UpdateBounds();
		void UpdateProjection();

	protected:
		float m_Size;
		float m_Left, m_Right, m_Bottom, m_Top;

		float m_NearClip = -1.0f;
		float m_FarClip = 1.0f;

		float m_AspectRatio;

		float m_ViewportWidth = 1920.0f;
		float m_ViewportHeight = 1080.0f;

	};

}
