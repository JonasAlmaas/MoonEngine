#pragma once

#include "Moon/Core/Renderer/Texture/Texture.h"

#include <glm/glm.hpp>


namespace Moon {

	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetUVCoords() const { return m_UVCoords; }

		/**
		 * Creates a subtexture from a texture. Used for example for textures sheets.
		 *
		 * @param texture - Just a Ref<Texture> to your texture sheet.
		 * @param spriteSize - glm::vec2 describing width and height of the sprite. Unit is pixles.
		 * @param spriteCoord - glm::vec2 describing the position of the sprite on the sheet. { 0, 0 } is top left.
		 */
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const glm::vec2& spriteSize, const glm::vec2& spriteCoord);

		/**
		 * Creates a subtexture from a texture. Used for example for textures sheets.
		 *
		 * @param texture - Just a Ref<Texture> to your texture sheet.
		 * @param spriteSize - glm::vec2 describing width and height of the sprite. Unit is pixles.
		 * @param Sprite index - This is a float just in case you want to do use it in a silly way.
		 */
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const glm::vec2& spriteSize, float spriteIndex);

		/**
		 * Creates a subtexture from a texture. Used for example for textures sheets.
		 * 
		 * @param Texture - Just a Ref<Texture>
		 * @param Sprite size - One single float describing the size of the sprite. Unit is pixles.
		 * @param Sprite index - This is a float just in case you want to do use it in a silly way.
		 */
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, float spriteSize, float spriteIndex);

		/**
		 * Creates a subtexture from a texture. Used for example for textures sheets.
		 *
		 * @param texture - Just a Ref<Texture> to your texture sheet.
		 * @param Sprite size - One single float describing the size of the sprite. Unit is pixles.
		 * @param spriteCoord - glm::vec2 describing the position of the sprite on the sheet. { 0, 0 } is top left.
		 */
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, float spriteSize, const glm::vec2& spriteCoord);

	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_UVCoords[4];

	};

}
