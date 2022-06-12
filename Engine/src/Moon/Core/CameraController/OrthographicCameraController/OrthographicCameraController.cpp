#include "mepch.h"
#include "Moon/Core/CameraController/OrthographicCameraController/OrthographicCameraController.h"

#include "Moon/Core/Application/Application.h"
#include "Moon/Core/Input/Input.h"
#include "Moon/Core/Input/Code/KeyCode.h"
#include "Moon/Core/Input/Code/MouseCode.h"


namespace Moon {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool moveWASD, bool moveMMB, bool rotation)
		: m_AspectRatio(aspectRatio), m_Rotation(rotation), m_MoveWASD(moveWASD), m_MoveMMB(moveMMB),
			m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (m_MoveMMB)
		{
			if (Input::IsMouseButtonPressed(Mouse::Button2))
			{
				auto [x, y] = Input::GetMousePosition();

				float a = m_ZoomLevel * 2.0f / (float)Application::Get().GetWindow().GetHeight();

				float distanceX = (m_LastMousePosition.x - x) * a;
				float distanceY = (m_LastMousePosition.y - y) * a;

				m_CameraPosition.x += distanceX * cos(glm::radians(m_CameraRotation));
				m_CameraPosition.y += distanceX * sin(glm::radians(m_CameraRotation));

				m_CameraPosition.x += distanceY * sin(glm::radians(m_CameraRotation));
				m_CameraPosition.y -= distanceY * cos(glm::radians(m_CameraRotation));
				
				m_LastMousePosition = { x, y };

				m_Camera.SetPosition(m_CameraPosition);
			}
		}

		if (m_MoveWASD)
		{
			if (Input::IsKeyPressed(Key::W))
			{
				m_CameraPosition.x -= m_CameraTranslationSpeed * m_ZoomLevel * ts * sin(glm::radians(m_CameraRotation));
				m_CameraPosition.y += m_CameraTranslationSpeed * m_ZoomLevel * ts * cos(glm::radians(m_CameraRotation));
			}

			if (Input::IsKeyPressed(Key::A))
			{
				m_CameraPosition.x -= m_CameraTranslationSpeed * m_ZoomLevel * ts * cos(glm::radians(m_CameraRotation));
				m_CameraPosition.y -= m_CameraTranslationSpeed * m_ZoomLevel * ts * sin(glm::radians(m_CameraRotation));
			}

			if (Input::IsKeyPressed(Key::S))
			{
				m_CameraPosition.x += m_CameraTranslationSpeed * m_ZoomLevel * ts * sin(glm::radians(m_CameraRotation));
				m_CameraPosition.y -= m_CameraTranslationSpeed * m_ZoomLevel * ts * cos(glm::radians(m_CameraRotation));
			}

			if (Input::IsKeyPressed(Key::D))
			{
				m_CameraPosition.x += m_CameraTranslationSpeed * m_ZoomLevel * ts * cos(glm::radians(m_CameraRotation));
				m_CameraPosition.y += m_CameraTranslationSpeed * m_ZoomLevel * ts * sin(glm::radians(m_CameraRotation));
			}
		}

		if (m_MoveMMB || m_MoveWASD)
			m_Camera.SetPosition(m_CameraPosition);

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;

			if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			m_Camera.SetRotationDegrees(m_CameraRotation);
		}
	}

	// ---- Event Handling ---- 

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseButtonPressedEvent>(ME_BIND_EVENT_FN(OrthographicCameraController::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(ME_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(ME_BIND_EVENT_FN(OrthographicCameraController::OnWindowResizeEvent));
	}

	bool OrthographicCameraController::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::Button2)
		{
			auto [x, y] = Input::GetMousePosition();
			m_LastMousePosition = { x, y };
		}
		return false;
	}

	bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}
