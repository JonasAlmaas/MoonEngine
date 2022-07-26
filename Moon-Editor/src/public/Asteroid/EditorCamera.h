#pragma once

using namespace Moon;


namespace Asteroid {

	class EditorCamera : public PerspectiveCamera
	{
	public:
		EditorCamera();
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		virtual ~EditorCamera() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

	private:
		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> GetPanSpeed() const;
		float GetZoomSpeed() const;

	private:
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;

		float m_RotationSpeed = 0.8f;
	};

}
