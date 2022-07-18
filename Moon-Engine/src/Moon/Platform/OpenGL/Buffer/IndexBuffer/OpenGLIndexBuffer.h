#pragma once

#include "Moon/Core/Renderer/Buffer/IndexBuffer/IndexBuffer.h"


namespace Moon {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual uint32_t GetCount() const override { return m_Count; };

	private:
		RendererID m_RendererID;
		uint32_t m_Count;
	};

}
