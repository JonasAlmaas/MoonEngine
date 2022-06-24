#include "mepch.h"
#include "Moon/Scene/Camera/SceneCamera.h"


namespace Moon {
	
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;

		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;

		m_PerspectiveFOV = verticalFov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::SetProjectionType(ProjectionType type)
	{
		m_ProjectionType = type;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspectiveVerticalFOV(float verticalFov)
	{
		m_PerspectiveFOV = glm::radians(verticalFov);
		RecalculateProjection();
	}

	void SceneCamera::SetPerspectiveNearClip(float nearClip)
	{
		m_PerspectiveNear = nearClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspectiveFarClip(float farClip)
	{
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographicSize(float size)
	{
		m_OrthographicSize = size;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographicNearClip(float nearClip)
	{
		m_OrthographicNear = nearClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographicFarClip(float farClip)
	{
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		switch (m_ProjectionType)
		{
			case ProjectionType::Perspective:
			{
				m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
				break;
			}
			case ProjectionType::Orthographic:
			{
				float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
				float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
				float orthoBottom = -m_OrthographicSize * 0.5f;
				float orthoTop = m_OrthographicSize * 0.5f;

				m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
				break;
			}
			default:
			{
				ME_CORE_ASSERT(false, "Unknown ProjectionType");
				break;
			}
		}
	}

}
