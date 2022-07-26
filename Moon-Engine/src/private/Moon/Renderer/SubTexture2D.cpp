#include "mepch.h"
#include "Moon/Renderer/SubTexture2D.h"


namespace Moon {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max, const glm::vec2& spriteSize)
		: m_Texture(texture), m_SpriteSize(spriteSize)
	{
		m_UVCoords[0] = { min.x, min.y };
		m_UVCoords[1] = { max.x, min.y };
		m_UVCoords[2] = { max.x, max.y };
		m_UVCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2& cellSize, const glm::vec2& spriteCoord, const glm::vec2& spriteSize)
	{
		ME_PROFILE_FUNCTION();

		float uvFactorX = (1.0f / ((float)texture->GetWidth() / cellSize.x));
		float uvFactorY = (1.0f / ((float)texture->GetHeight() / cellSize.y));

		glm::vec2 min = { spriteCoord.x * uvFactorX, 1.0f - (spriteCoord.y + spriteSize.y) * uvFactorY };
		glm::vec2 max = { (spriteCoord.x + spriteSize.x) * uvFactorX, 1.0f - (spriteCoord.y) * uvFactorY };

		return CreateRef<SubTexture2D>(texture, min, max, spriteSize);
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2& cellSize, float spriteIndex, const glm::vec2& spriteSize)
	{
		ME_PROFILE_FUNCTION();

		float horizontalSpriteCount = (float)texture->GetWidth() / cellSize.x;
		float a = spriteIndex / horizontalSpriteCount;
		float x = (a - floor(a)) * horizontalSpriteCount;
		float y = floor(a);

		return Create(texture, cellSize, { x, y }, spriteSize);
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, float cellSize, const glm::vec2& spriteCoord, const glm::vec2& spriteSize)
	{
		return Create(texture, { cellSize, cellSize }, spriteCoord, spriteSize);
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, float cellSize, float spriteIndex, const glm::vec2& spriteSize)
	{
		return Create(texture, { cellSize, cellSize }, spriteIndex, spriteSize);
	}

}
