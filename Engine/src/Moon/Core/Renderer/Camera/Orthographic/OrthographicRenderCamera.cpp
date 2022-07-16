#include "mepch.h"
#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicRenderCamera.h"


namespace Moon {

	OrthographicRenderCamera::OrthographicRenderCamera(float size, float aspectRatio)
		: m_Left(-size * aspectRatio * 0.5f), m_Right(size* aspectRatio * 0.5f), m_Bottom(-size * 0.5f), m_Top(size * 0.5),
			m_Size(size), m_AspectRatio(aspectRatio)
	{
		UpdateProjection();
	}

	OrthographicRenderCamera::OrthographicRenderCamera(float left, float right, float bottom, float top)
		: m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top),
			m_Size((bottom - top) / 2.0f), m_AspectRatio(m_Right - m_Left / m_Bottom - m_Top)
	{
		UpdateProjection();
	}

	void OrthographicRenderCamera::SetSize(float size)
	{
		m_Size = size;

		UpdateBounds();
		UpdateProjection();
	}

	void OrthographicRenderCamera::SetBounds(float left, float right, float bottom, float top)
	{
		m_AspectRatio = m_Right - m_Left / m_Bottom - m_Top;

		UpdateBounds();
		UpdateProjection();
	}

	void OrthographicRenderCamera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;

		UpdateBounds();
		UpdateProjection();
	}

	void OrthographicRenderCamera::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;

		UpdateBounds();
		UpdateProjection();
	}

	void OrthographicRenderCamera::SetNearClip(float nearClip)
	{
		m_NearClip = nearClip;
		UpdateProjection();
	}

	void OrthographicRenderCamera::SetFarClip(float farClip)
	{
		m_FarClip = farClip;
		UpdateProjection();
	}

	void OrthographicRenderCamera::UpdateBounds()
	{
		m_Left = -m_Size * m_AspectRatio * 0.5f;;
		m_Right = m_Size * m_AspectRatio * 0.5f;
		m_Bottom = -m_AspectRatio * 0.5f;
		m_Top = m_AspectRatio * 0.5f;
	}

	void OrthographicRenderCamera::UpdateProjection()
	{
		m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearClip, m_FarClip);
	}

}
