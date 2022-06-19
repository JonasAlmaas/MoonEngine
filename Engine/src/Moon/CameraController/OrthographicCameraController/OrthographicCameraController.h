#pragma once

#include "Moon/Core/Event/ApplicationEvent.h"
#include "Moon/Core/Event/MouseEvent.h"
#include "Moon/Core/Renderer/Camera/Orthographic/OrthographicCamera.h"


namespace Moon {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool moveWASD = false, bool moveMMB = false, bool zoom = false, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(uint32_t width, uint32_t height);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

		inline float GetZoomLevel() const { return m_ZoomLevel; }

		void SetZoomLevel(float level)
		{
			m_ZoomLevel = level;
			CalculateView();
		}

		inline const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

	private:
		inline void CalculateView();

		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCameraBounds m_Bounds;

		bool m_MoveWASD;
		bool m_MoveMMB;
		bool m_Zoom;
		bool m_Rotation;

		OrthographicCamera m_Camera;

		glm::vec2 m_LastMousePosition = { 0.0f, 0.0f };
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 50.0f;

	};

}
