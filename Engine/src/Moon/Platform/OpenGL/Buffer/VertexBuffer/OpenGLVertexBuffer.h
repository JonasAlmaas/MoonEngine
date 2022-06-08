#pragma once

#include "Moon/Core/Renderer/Buffer/VertexBuffer/VertexBuffer.h"


namespace Moon {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

	private:
		uint32_t m_RendererID;

	};

}
