#include "mepch.h"
#include "Moon/GameFramework/Actor.h"


namespace Moon {
	
	// -- Transform --
	void Actor::SetTransform(const glm::mat4& transform)
	{
		Math::DecomposeTransform(transform, m_Position, m_Rotation, m_Scale);
	}

	glm::mat4 Actor::GetTransform() const
	{
		return glm::translate(glm::mat4(1.0f), m_Position)
			* glm::toMat4(glm::quat(m_Rotation))
			* glm::scale(glm::mat4(1.0f), m_Scale);
	}

	// -- Directions --
	glm::vec3 Actor::GetUpVector() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Actor::GetRightVector() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 Actor::GetForwardVector() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat Actor::GetOrientation() const
	{
		// TODO: Not sure if all of these shoudl be negative
		return glm::quat(glm::vec3(-m_Pitch, -m_Roll, -m_Yaw));
	}

}
