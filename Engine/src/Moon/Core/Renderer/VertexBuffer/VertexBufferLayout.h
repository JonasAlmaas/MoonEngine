#pragma once

#include "Moon/Core/Renderer/VertexBuffer/VertexBufferElement.h"


namespace Moon {

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() = default;

		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements);

		inline uint32_t GetStride() const { return m_Stride; }
		std::vector<VertexBufferElement> GetElements() const { return m_Elements; }

		std::vector<VertexBufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<VertexBufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<VertexBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<VertexBufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride();

	private:
		std::vector<VertexBufferElement> m_Elements;
		uint32_t m_Stride;

	};

}
