#pragma once

#include "Moon/Core/Renderer/Texture/Texture.h"


namespace Moon {

	struct SpriteRendererComponent
	{
		Moon::Color Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		Ref<Texture2D> Texture;
		glm::vec2 TileFactor = { 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Moon::Color& Color)
			: Color(Color) {}
	};

}

