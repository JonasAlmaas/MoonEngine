#include "aopch.h"
#include "Asteroid/EditorCamera.h"

#include "Asteroid/EditorState.h"


namespace Asteroid {

	EditorCamera::EditorCamera()
	{
		m_Pitch = glm::radians(-45.0f);
		m_Position = CalculatePosition();
		UpdateView();
	}

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: PerspectiveCamera(fov, aspectRatio, nearClip, farClip)
	{
		m_Position = CalculatePosition();
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;

		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);

			m_Position = CalculatePosition();
			UpdateView();
		}
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = GetPanSpeed();
		m_FocalPoint += -GetRightVector() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpVector() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpVector().z < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * m_RotationSpeed;
		m_Pitch += delta.y * m_RotationSpeed;
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * GetZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardVector();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardVector() * m_Distance;
	}

	std::pair<float, float> EditorCamera::GetPanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::GetZoomSpeed() const
	{
		float distance = m_Distance * 0.3f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	// ---- Event Handling ----

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ME_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
		dispatcher.Dispatch<KeyPressedEvent>(ME_BIND_EVENT_FN(EditorCamera::OnKeyPressed));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);

		m_Position = CalculatePosition();
		UpdateView();

		return false;
	}

	bool EditorCamera::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;

		switch (e.GetKeyCode())
		{
			case Key::F:
			{
				// Set focus to the origion of the selection context
				Entity selectionContext = EditorState::GetActiveScene()->GetSelectionContext();
				if (selectionContext)
				{
					m_FocalPoint = selectionContext.GetComponent<TransformComponent>().Translation;
					m_Position = CalculatePosition();
					UpdateView();
				}
				break;
			}
			default:
				break;
		}

		return false;
	}

}
