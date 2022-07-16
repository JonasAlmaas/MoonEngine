#pragma once

#include "Moon/Scene/Camera/SceneCamera.h"


namespace Moon {

	struct CameraComponent
	{
		Ref<SceneCamera> Camera = CreateRef<SceneCamera>();

		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

}
