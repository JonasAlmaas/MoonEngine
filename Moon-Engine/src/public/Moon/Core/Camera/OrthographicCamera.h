#pragma once

#include "Moon/GameFramework/Actor.h"
#include "Moon/Core/Renderer/Camera/OrthographicRenderCamera.h"


namespace Moon {

	class OrthographicCamera : public Actor, public OrthographicRenderCamera
	{
	public:
		OrthographicCamera() = default;
		OrthographicCamera(float size);
		OrthographicCamera(float size, float aspectRatio);
		OrthographicCamera(float left, float right, float bottom, float top);

		virtual ~OrthographicCamera() = default;

		// -- Positions --
		virtual void SetPosition(const glm::vec3& pos) override;

		// -- Rotaion --
		void SetRotationDegrees(float rotationDegrees);
		void SetRotationRadians(float rotationRadians);
		inline float GetRotationDegrees() const { return glm::degrees(m_Rotation.z); }
		inline float GetRotationRadians() const { return m_Rotation.z; }

	protected:
		void UpdateView();

	private:
		// Remove methods from the API
		using Actor::SetRotation;
		using Actor::GetRotation;
		using Actor::SetPitchDegrees;
		using Actor::GetPitchDegrees;
		using Actor::SetPitchRadians;
		using Actor::GetPitchRadians;
		using Actor::SetYawDegrees;
		using Actor::SetYawRadians;
		using Actor::GetYawDegrees;
		using Actor::GetYawRadians;
		using Actor::SetRollDegrees;
		using Actor::SetRollRadians;
		using Actor::GetRollDegrees;
		using Actor::GetRollRadians;
		using Actor::SetScale;
		using Actor::GetScale;

	};

}
