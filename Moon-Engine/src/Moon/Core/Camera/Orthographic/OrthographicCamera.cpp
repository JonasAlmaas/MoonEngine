#include "mepch.h"
#include "Moon/Core/Camera/Orthographic/OrthographicCamera.h"


namespace Moon {

	OrthographicCamera::OrthographicCamera(float size, float aspectRatio)
		: OrthographicRenderCamera(size, aspectRatio)
	{
		UpdateView();
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: OrthographicRenderCamera(left, right, bottom, top)
	{
		UpdateView();
	}

	void OrthographicCamera::SetPosition(const glm::vec3& pos)
	{
		Actor::SetPosition(pos);
		UpdateView();
	}

	void OrthographicCamera::SetRotationDegrees(float rotationDegrees)
	{
		m_Rotation.z = glm::radians(rotationDegrees);
		UpdateView();
	}

	void OrthographicCamera::SetRotationRadians(float rotationRadians)
	{
		m_Rotation.z = rotationRadians;
		UpdateView();
	}

	void OrthographicCamera::UpdateView()
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_Rotation.z, { 0.0f, 0.0f, 1.0f });
		m_View = glm::inverse(translation * rotation);
	}

}
