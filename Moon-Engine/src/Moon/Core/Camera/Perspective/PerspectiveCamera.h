#pragma once

#include "Moon/GameFramework/Actor/Actor.h"
#include "Moon/Core/Renderer/Camera/Perspective/PerspectiveRenderCamera.h"


namespace Moon {

	class PerspectiveCamera : public Actor, public PerspectiveRenderCamera
	{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);

		virtual ~PerspectiveCamera() = default;

		virtual void SetPosition(const glm::vec3& pos) override;
		
		virtual void SetRotation(const glm::vec3& rotation) override;

		virtual void SetPitchDegrees(float pitchDegrees) override;
		virtual void SetPitchRadians(float pitchRadians) override;
		
		virtual void SetYawDegrees(float yawDegrees) override;
		virtual void SetYawRadians(float yawRadians) override;
		
		virtual void SetRollDegrees(float rollDegrees) override;
		virtual void SetRollRadians(float rollRadians) override;

	protected:
		void UpdateView();

	};

}
