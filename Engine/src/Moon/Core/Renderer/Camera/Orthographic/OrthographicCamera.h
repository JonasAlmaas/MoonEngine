#pragma once

#include <glm/glm.hpp>


namespace Moon {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
	
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3 position);

		float GetRotationDegrees() const { return m_RotationDegrees; }
		void SetRotationDegrees(float rotation);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f};
		float m_RotationDegrees = 0.0f;

	};

}
