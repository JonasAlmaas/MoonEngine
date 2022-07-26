#pragma once

#include "Moon/Core/Renderer/Texture.h"


namespace Moon {

	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max, const glm::vec2& spriteSize = { 1.0f, 1.0f });

		const Ref<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetUVCoords() const { return m_UVCoords; }
		const glm::vec2 GetSpriteSize() const { return m_SpriteSize; }

		/**
		 * Creates a subtexture from a texture. Used for example for textures sheets.
		 *
		 * @param texture - Just a Ref<Texture> to your texture sheet.
		 * @param cellSize - glm::vec2 describing width and height of the sprite. Unit is pixles.
		 * @param spriteCoord - glm::vec2 describing the position of the sprite on the sheet. { 0, 0 } is top left.
		 * @param spriteSize - glm::vec2 describing how many cells this sprites takes. { 1, 2 } would be one cell horizonatl and two vertical.
		 */
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const glm::vec2& cellSize, const glm::vec2& spriteCoord, const glm::vec2& spriteSize = { 1.0f, 1.0f });

		/**
		 * Creates a subtexture from a texture. Used for example for textures sheets.
		 *
		 * @param texture - Just a Ref<Texture> to your texture sheet.
		 * @param cellSize - glm::vec2 describing width and height of the sprite. Unit is pixles.
		 * @param spriteIndex - This is a float just in case you want to do use it in a silly way.
		 * @param spriteSize - glm::vec2 describing how many cells this sprites takes. { 1, 2 } would be one cell horizonatl and two vertical.
		 */
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const glm::vec2& cellSize, float spriteIndex, const glm::vec2& spriteSize = { 1.0f, 1.0f });

		/**
		 * Creates a subtexture from a texture. Used for example for textures sheets.
		 *
		 * @param texture - Just a Ref<Texture> to your texture sheet.
		 * @param cellSize - One single float describing the size of a single cell. Unit is pixles.'
		 * @param spriteCoord - glm::vec2 describing the position of the sprite on the sheet. { 0, 0 } is top left.
		 * @param spriteSize - glm::vec2 describing how many cells this sprites takes. { 1, 2 } would be one cell horizonatl and two vertical.
		 */
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, float cellSize, const glm::vec2& spriteCoord, const glm::vec2& spriteSize = { 1.0f, 1.0f });

		/**
		 * Creates a subtexture from a texture. Used for example for textures sheets.
		 *
		 * @param texture - Just a Ref<Texture> to your texture sheet.
		 * @param cellSize - One single float describing the size of a single cell. Unit is pixles.
		 * @param spriteIndex - This is a float just in case you want to do use it in a silly way.
		 * @param spriteSize - glm::vec2 describing how many cells this sprites takes. { 1, 2 } would be one cell horizonatl and two vertical.
		 */
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, float cellSize, float spriteIndex, const glm::vec2& spriteSize = { 1.0f, 1.0f });

	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_UVCoords[4];
		glm::vec2 m_SpriteSize;

	};

}
