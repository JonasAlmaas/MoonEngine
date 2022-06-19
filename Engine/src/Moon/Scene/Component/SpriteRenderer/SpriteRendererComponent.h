#pragma once

#include "Moon/Core/Type/Color.h"

#include <glm/glm.hpp>


namespace Moon {

	struct SpriteRendererComponent
	{
		Color Tint{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Color& tint)
			: Tint(tint) {}

		operator Color& () { return Tint; }
		operator const Color& () const { return Tint; }
	};

}
