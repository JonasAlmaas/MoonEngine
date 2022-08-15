#include "mepch.h"
#include "Moon/Renderer/TextSurface.h"
#include "Moon/Asset/Font/MSDFData.h"


namespace Moon {

	struct TextVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 UV;
	};

	//From https://stackoverflow.com/questions/31302506/stdu32string-conversion-to-from-stdstring-and-stdu16string
	static std::u32string To_UTF32(const std::string& s)
	{
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
		return conv.from_bytes(s);
	}

	TextSurface::TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::mat4& transform, const Color& color)
		: m_Width(0), m_Height(0)
	{
		Setup(text, font, transform, color);
	}

	TextSurface::TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec3& position, const glm::vec2& scale, const glm::vec3 rotation, const Color& color)
		: m_Width(0), m_Height(0)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(glm::quat(rotation)) * glm::scale(glm::mat4(1.0f), { scale, 0.0f });
		Setup(text, font, transform, color);
	}

	TextSurface::TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec3& position, const glm::vec2& scale, const Color& color)
		: m_Width(0), m_Height(0)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { scale, 0.0f });
		Setup(text, font, transform, color);
	}

	TextSurface::TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec3& position, float scale, const glm::vec3 rotation, const Color& color)
		: m_Width(0), m_Height(0)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(glm::quat(rotation)) * glm::scale(glm::mat4(1.0f), { scale, scale, 0.0f });
		Setup(text, font, transform, color);
	}

	TextSurface::TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec3& position, float scale, const Color& color)
		: m_Width(0), m_Height(0)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { scale, scale, 0.0f });
		Setup(text, font, transform, color);
	}

	TextSurface::TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec2& position, const glm::vec2& scale, float rotation, const Color& color)
		: m_Width(0), m_Height(0)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position, 0.0f }) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { scale, 0.0f });
		Setup(text, font, transform, color);
	}

	TextSurface::TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec2& position, const glm::vec2& scale, const Color& color)
		: m_Width(0), m_Height(0)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position, 0.0f }) * glm::scale(glm::mat4(1.0f), { scale, 0.0f });
		Setup(text, font, transform, color);
	}

	TextSurface::TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec2& position, float scale, float rotation, const Color& color)
		: m_Width(0), m_Height(0)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position, 0.0f }) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { scale, scale, 0.0f });
		Setup(text, font, transform, color);
	}

	TextSurface::TextSurface(const std::string& text, const Ref<FontAsset>& font, const glm::vec2& position, float scale, const Color& color)
		: m_Width(0), m_Height(0)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position, 0.0f }) * glm::scale(glm::mat4(1.0f), { scale, scale, 0.0f });
		Setup(text, font, transform, color);
	}

	void TextSurface::Setup(const std::string& text, const Ref<FontAsset>& font, const glm::mat4& transform, const Color& color)
	{
		if (text.empty())
			return;

		Ref<Texture2D> fontAtlas = font->GetFontAtlas();
		ME_CORE_ASSERT(fontAtlas, "Failed to get font atlas!");

		std::u32string utf32string = To_UTF32(text);

		// Get visible character count (not space nor new line)
		uint32_t charCount = 0;
		for (int i = 0; i < utf32string.size(); i++)
		{
			char32_t character = utf32string[i];

			if (character == '\n')
				continue;
			if (character == ' ')
				continue;

			charCount++;
		}

		uint32_t vertexCount = charCount * 4;
		uint32_t indexCount = charCount * 6;

		// Allocate buffer for vertex data
		TextVertex* vertexBufferBase = new TextVertex[vertexCount];
		TextVertex* vertexBufferPtr = vertexBufferBase;

		auto& fontGeometry = font->GetMSDFData()->FontGeometry;
		const auto& metrics = fontGeometry.getMetrics();

		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = 0.0;

		float scaledLineHeight = (float)fsScale * (glm::vec4(0.0f, metrics.lineHeight, 0.0f, 1.0f) * transform).y;
		m_LineHeight = scaledLineHeight * 0.5f;
		m_Height = m_LineHeight;

		for (int i = 0; i < utf32string.size(); i++)
		{
			// New line
			char32_t character = utf32string[i];
			if (character == '\n')
			{
				x = 0;
				y -= fsScale * metrics.lineHeight;
				m_Height += scaledLineHeight;
				continue;
			}


			// Get the glyph geometry
			auto glyph = fontGeometry.getGlyph(character);
			if (!glyph)
				glyph = fontGeometry.getGlyph('?');
			if (!glyph)
				continue;

			// Space
			if (character == ' ')
			{
				double advance = glyph->getAdvance();
				fontGeometry.getAdvance(advance, character, utf32string[i + 1]);
				x += fsScale * advance;

				float scaledLineWidth = (glm::vec4((float)x, 0.0f, 0.0f, 1.0f) * transform).x;
				m_Width = glm::max<float>(m_Width, scaledLineWidth);

				continue;
			}

			double l, b, r, t;
			glyph->getQuadAtlasBounds(l, b, r, t);

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);

			pl *= fsScale;
			pb *= fsScale;
			pr *= fsScale;
			pt *= fsScale;

			pl += x;
			pb += y;
			pr += x;
			pt += y;

			double texelWidth = 1. / fontAtlas->GetWidth();
			double texelHeight = 1. / fontAtlas->GetHeight();

			l *= texelWidth;
			b *= texelHeight;
			r *= texelWidth;
			t *= texelHeight;

			// Bottom left
			vertexBufferPtr->Position = transform * glm::vec4(pl, pb, 0.0f, 1.0f);
			vertexBufferPtr->Color = color;
			vertexBufferPtr->UV = { l, b };
			vertexBufferPtr++;

			// Bottom right
			vertexBufferPtr->Color = color;
			vertexBufferPtr->Position = transform * glm::vec4(pr, pb, 0.0f, 1.0f);
			vertexBufferPtr->UV = { r, b };
			vertexBufferPtr++;

			// Top Left
			vertexBufferPtr->Position = transform * glm::vec4(pl, pt, 0.0f, 1.0f);
			vertexBufferPtr->Color = color;
			vertexBufferPtr->UV = { l, t };
			vertexBufferPtr++;

			// Top right
			vertexBufferPtr->Position = transform * glm::vec4(pr, pt, 0.0f, 1.0f);
			vertexBufferPtr->Color = color;
			vertexBufferPtr->UV = { r, t };
			vertexBufferPtr++;

			double advance = glyph->getAdvance();
			fontGeometry.getAdvance(advance, character, utf32string[i + 1]);
			x += fsScale * advance;

			float scaledLineWidth = (glm::vec4((float)x, 0.0f, 0.0f, 1.0f) * transform).x;
			m_Width = glm::max<float>(m_Width, scaledLineWidth);
		}

		m_VertexArray = VertexArray::Create();

		Ref<VertexBuffer> vb = VertexBuffer::Create(vertexBufferBase, vertexCount * sizeof(TextVertex));

		vb->SetLayout({
			{ ShaderDataType::Float3, "Position"},
			{ ShaderDataType::Float4, "Color"},
			{ ShaderDataType::Float2, "UV"},
			});

		m_VertexArray->AddVertexBuffer(vb);

		// Generate indices
		uint32_t* indices = new uint32_t[indexCount];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < indexCount; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 1;
			indices[i + 5] = offset + 3;

			offset += 4;
		}

		Ref<IndexBuffer> ib = IndexBuffer::Create(indices, indexCount);
		m_VertexArray->SetIndexBuffer(ib);

		delete[] indices;
		delete[] vertexBufferBase;
	}

}
