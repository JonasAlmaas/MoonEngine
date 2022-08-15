#pragma once

#include "Moon/Core/Renderer/VertexArray.h"
#include "Moon/Core/Type/Color.h"
#include "Moon/Asset/FontAsset.h"


namespace Moon {

	class TextSurface
	{
	public:
		TextSurface() = default;
		TextSurface(const std::string& text, const Ref<FontAsset> font, float textSize, const Color& color);
		~TextSurface() = default;

		inline float GetWidth() const { return m_Width; }
		inline float GetHeight() const { return m_Height; }
		inline glm::vec2 GetSize() const { return { m_Width, m_Height }; }

		inline float GetLineHeight() const { return m_LineHeight; }

		Ref<FontAsset> GetFont() const { return m_Font; }
		Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }

	private:
		float m_Width, m_Height, m_LineHeight;
		Ref<FontAsset> m_Font;
		Ref<VertexArray> m_VertexArray;

	};

}
