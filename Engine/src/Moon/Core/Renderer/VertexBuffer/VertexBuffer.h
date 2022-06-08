#pragma once

#include "Moon/Core/Renderer/VertexBuffer/VertexBufferLayout.h"


namespace Moon {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const VertexBufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const VertexBufferLayout& layout) = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);

	};

}
