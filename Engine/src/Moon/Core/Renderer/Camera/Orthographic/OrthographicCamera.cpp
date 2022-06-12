#include "mepch.h"
#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Moon {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3 position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetRotationDegrees(float rotation)
	{
		m_RotationRadians = glm::radians(rotation);
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetRotationRadians(float rotation)
	{
		m_RotationRadians = rotation;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), m_RotationRadians, glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
