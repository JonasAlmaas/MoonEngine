#pragma once

#include "Moon/Core/Event/ApplicationEvent.h"
#include "Moon/Core/Event/MouseEvent.h"
#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicCamera.h"
#include "Moon/Core/Type/Timestep.h"


namespace Moon {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool moveWASD = false, bool moveMMB = false, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		bool m_MoveWASD;
		bool m_MoveMMB;
		bool m_Rotation;

		OrthographicCamera m_Camera;

		glm::vec2 m_LastMousePosition = { 0.0f, 0.0f };
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 50.0f;

	};

}
