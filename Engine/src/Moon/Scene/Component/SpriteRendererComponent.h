#pragma once

#include "Moon/Core/Type/Color.h"

#include <glm/glm.hpp>


namespace Moon {

	struct SpriteRendererComponent
	{
		Moon::Color Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Moon::Color& Color)
			: Color(Color) {}

		operator Moon::Color& () { return Color; }
		operator const Moon::Color& () const { return Color; }
	};

}
