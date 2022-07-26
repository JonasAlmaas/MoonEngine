#include "mepch.h"
#include "Moon/Core/Renderer/Buffer/VertexBuffer/VertexBufferLayout.h"


namespace Moon {
	
	VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
		: m_Elements(elements)
	{
		CalculateOffsetAndStride();
	}

	void VertexBufferLayout::CalculateOffsetAndStride()
	{
		ME_PROFILE_FUNCTION();

		uint32_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

}
