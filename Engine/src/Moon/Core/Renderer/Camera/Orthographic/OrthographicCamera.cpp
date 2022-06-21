#include "mepch.h"
#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicCamera.h"


namespace Moon {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		ME_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		ME_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3 position)
	{
		ME_PROFILE_FUNCTION();

		m_Position = position;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetRotationDegrees(float rotation)
	{
		ME_PROFILE_FUNCTION();

		m_RotationRadians = glm::radians(rotation);
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetRotationRadians(float rotation)
	{
		ME_PROFILE_FUNCTION();

		m_RotationRadians = rotation;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		ME_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), m_RotationRadians, { 0.0f, 0.0f, 1.0f });
		m_ViewMatrix = glm::inverse(transform);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
