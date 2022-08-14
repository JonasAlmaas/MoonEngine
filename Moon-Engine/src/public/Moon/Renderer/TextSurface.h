#pragma once

#include "Moon/Core/Renderer/VertexArray.h"
#include "Moon/Core/Type/Color.h"
#include "Moon/Asset/FontAsset.h"


namespace Moon {

	class TextSurface
	{
	public:
		TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::mat4& transform, const Color& color);

		TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec3& position, const glm::vec3& size, const glm::vec3 rotation, const Color& color);
		TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec3& position, const glm::vec3& size, const Color& color);
		TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec3& position, float size, const glm::vec3 rotation, const Color& color);
		TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec3& position, float size, const Color& color);

		TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec2& position, const glm::vec2& size, float rotation, const Color& color);
		TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec2& position, const glm::vec2& size, const Color& color);
		TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec2& position, float size, float rotation, const Color& color);
		TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec2& position, float size, const Color& color);

		~TextSurface() = default;

		inline float GetWidth() const { return m_Width; }
		inline float GetHeight() const { return m_Height; }
		inline glm::vec2 GetSize() const { return { m_Width, m_Height }; }

		inline float GetLineHeight() const { return m_LineHeight; }

		Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }

	private:
		void Setup(const std::string& text, const Ref<FontAsset>& font, const glm::mat4& transform, const Color& color);

	private:
		float m_Width, m_Height, m_LineHeight;

		Ref<VertexArray> m_VertexArray;

	};

}
