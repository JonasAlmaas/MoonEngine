#include "mepch.h"
#include "Moon/Core/Camera/Perspective/PerspectiveCamera.h"


namespace Moon {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: PerspectiveRenderCamera(fov, aspectRatio, nearClip, farClip)
	{
		UpdateView();
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& pos)
	{
		m_Position = pos;
		UpdateView();
	}

	void PerspectiveCamera::SetPitchDegrees(float pitchDegrees)
	{
		m_Pitch = glm::radians(pitchDegrees);
		UpdateView();
	}

	void PerspectiveCamera::SetPitchRadians(float pitchRadians)
	{
		m_Pitch = pitchRadians;
		UpdateView();
	}

	void PerspectiveCamera::SetYawDegrees(float yawDegrees)
	{
		m_Yaw = glm::radians(yawDegrees);
		UpdateView();
	}

	void PerspectiveCamera::SetYawRadians(float yawRadians)
	{
		m_Yaw = yawRadians;
		UpdateView();
	}

	void PerspectiveCamera::SetRollDegrees(float rollDegrees)
	{
		m_Roll = glm::radians(rollDegrees);
		UpdateView();
	}

	void PerspectiveCamera::SetRollRadians(float rollRadians)
	{
		m_Roll = rollRadians;
		UpdateView();
	}

	glm::vec3 PerspectiveCamera::GetUpVector() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 PerspectiveCamera::GetRightVector() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 PerspectiveCamera::GetForwardVector() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat PerspectiveCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Roll, -m_Yaw));
	}

	void PerspectiveCamera::UpdateView()
	{
		glm::quat orientation = GetOrientation();
		m_View = glm::inverse(glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation));
	}

}
