#pragma once

#include "Moon/Core/Renderer/Buffer/VertexBuffer.h"


namespace Moon {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(const void* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const VertexBufferLayout& GetLayout() const override { return m_Layout; };
		virtual void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; };

	private:
		RendererID m_RendererID;
		VertexBufferLayout m_Layout;

	};

}
