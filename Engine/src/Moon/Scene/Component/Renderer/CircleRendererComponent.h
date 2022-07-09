#pragma once

#include "Moon/Core/Type/Color.h"


namespace Moon {

	struct CircleRendererComponent
	{
		// How donuted the circle should be. 1 is not donut at all.
		float Thickness = 1.0f;
		float Fade = 0.005f;
		Moon::Color Color;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

}
