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

	TextSurface::TextSurface(const std::string& text, const Ref<FontAsset> font, float textSize, const Color& color)
		: m_Width(0), m_Height(0), m_Font(font)
	{
		if (text.empty())
			return;

		Ref<Texture2D> fontAtlas = font->GetFontAtlas();
		ME_CORE_ASSERT(fontAtlas, "Failed to get font atlas!");

		std::u32string utf32string = To_UTF32(text);

		// Get visible character count (exclude space and new line characters)
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

		float scaledLineHeight = (float)(fsScale * metrics.lineHeight) * textSize;
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
				m_Width = glm::max<float>(m_Width, ((float)x * textSize));
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
			vertexBufferPtr->Position = glm::vec3(pl, pb, 0.0f) * textSize;
			vertexBufferPtr->Color = color;
			vertexBufferPtr->UV = { l, b };
			vertexBufferPtr++;

			// Bottom right
			vertexBufferPtr->Position = glm::vec3(pr, pb, 0.0f) * textSize;
			vertexBufferPtr->Color = color;
			vertexBufferPtr->UV = { r, b };
			vertexBufferPtr++;

			// Top Left
			vertexBufferPtr->Position = glm::vec3(pl, pt, 0.0f) * textSize;
			vertexBufferPtr->Color = color;
			vertexBufferPtr->UV = { l, t };
			vertexBufferPtr++;

			// Top right
			vertexBufferPtr->Position = glm::vec3(pr, pt, 0.0f) * textSize;
			vertexBufferPtr->Color = color;
			vertexBufferPtr->UV = { r, t };
			vertexBufferPtr++;

			double advance = glyph->getAdvance();
			fontGeometry.getAdvance(advance, character, utf32string[i + 1]);
			x += fsScale * advance;
			m_Width = glm::max<float>(m_Width, ((float)x * textSize));
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
