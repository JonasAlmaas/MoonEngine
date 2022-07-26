#include "mepch.h"
#include "Moon/Core/Renderer/Camera/PerspectiveRenderCamera.h"


namespace Moon {

	PerspectiveRenderCamera::PerspectiveRenderCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		UpdateProjection();
	}

	void PerspectiveRenderCamera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;

		UpdateProjection();
	}

	void PerspectiveRenderCamera::SetFOV(float fov)
	{
		m_FOV = fov;
		UpdateProjection();
	}

	void PerspectiveRenderCamera::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		UpdateProjection();
	}

	void PerspectiveRenderCamera::SetNearClip(float nearClip)
	{
		m_NearClip = nearClip;
		UpdateProjection();
	}

	void PerspectiveRenderCamera::SetFarClip(float farClip)
	{
		m_FarClip = farClip;
		UpdateProjection();
	}

	void PerspectiveRenderCamera::UpdateProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

}
