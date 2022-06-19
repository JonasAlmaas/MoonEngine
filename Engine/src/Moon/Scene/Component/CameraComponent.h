#pragma once

#include "Moon/Core/Renderer/Camera/Camera.h"


namespace Moon {

	struct CameraComponent
	{
		Moon::Camera Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			: Camera(projection) {}

		operator const Moon::Camera& () { return Camera; }
	};

}
