#include "mepch.h"
#include "Moon/Renderer/Texture/SubTexture2D.h"


namespace Moon {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_UVCoords[0] = { min.x, min.y };
		m_UVCoords[1] = { max.x, min.y };
		m_UVCoords[2] = { max.x, max.y };
		m_UVCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2& spriteSize, const glm::vec2& spriteCoord)
	{
		ME_PROFILE_FUNCTION();

		float uvFactorX = (1.0f / ((float)texture->GetWidth() / spriteSize.x));
		float uvFactorY = (1.0f / ((float)texture->GetHeight() / spriteSize.y));

		glm::vec2 min = { spriteCoord.x * uvFactorX, 1.0f - (spriteCoord.y + 1.0f) * uvFactorY };
		glm::vec2 max = { (spriteCoord.x + 1.0f) * uvFactorX, 1.0f - (spriteCoord.y) * uvFactorY };

		return CreateRef<SubTexture2D>(texture, min, max);
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2& spriteSize, float spriteIndex)
	{
		ME_PROFILE_FUNCTION();

		float horizontalSpriteCount = (float)texture->GetWidth() / spriteSize.x;
		float a = spriteIndex / horizontalSpriteCount;
		float x = (a - floor(a)) * horizontalSpriteCount;
		float y = floor(a);

		return Create(texture, spriteSize, { x, y });
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, float spriteSize, float spriteIndex)
	{
		return Create(texture, { spriteSize , spriteSize }, spriteIndex);
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, float spriteSize, const glm::vec2& spriteCoord)
	{
		return Create(texture, { spriteSize , spriteSize }, spriteCoord);
	}

}
