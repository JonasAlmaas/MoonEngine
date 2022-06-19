#pragma once

#include "Moon/Scene/Camera/SceneCamera.h"


namespace Moon {

	struct CameraComponent
	{
		SceneCamera Camera;

		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		operator const SceneCamera& () { return Camera; }
	};

}
