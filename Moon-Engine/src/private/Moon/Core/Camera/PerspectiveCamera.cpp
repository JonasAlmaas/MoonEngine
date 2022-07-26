#include "mepch.h"
#include "Moon/Core/Camera/PerspectiveCamera.h"


namespace Moon {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: PerspectiveRenderCamera(fov, aspectRatio, nearClip, farClip)
	{
		UpdateView();
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& pos)
	{
		Actor::SetPosition(pos);
		UpdateView();
	}

	void PerspectiveCamera::SetRotation(const glm::vec3& rotation)
	{
		Actor::SetRotation(rotation);
		UpdateView();
	}

	void PerspectiveCamera::SetPitchDegrees(float pitchDegrees)
	{
		Actor::SetPitchDegrees(pitchDegrees);
		UpdateView();
	}

	void PerspectiveCamera::SetPitchRadians(float pitchRadians)
	{
		Actor::SetPitchRadians(pitchRadians);
		UpdateView();
	}

	void PerspectiveCamera::SetYawDegrees(float yawDegrees)
	{
		Actor::SetYawDegrees(yawDegrees);
		UpdateView();
	}

	void PerspectiveCamera::SetYawRadians(float yawRadians)
	{
		Actor::SetYawRadians(yawRadians);
		UpdateView();
	}

	void PerspectiveCamera::SetRollDegrees(float rollDegrees)
	{
		Actor::SetRollDegrees(rollDegrees);
		UpdateView();
	}

	void PerspectiveCamera::SetRollRadians(float rollRadians)
	{
		Actor::SetRollRadians(rollRadians);
		UpdateView();
	}

	void PerspectiveCamera::UpdateView()
	{
		glm::quat orientation = GetOrientation();
		m_View = glm::inverse(glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation));
	}

}
