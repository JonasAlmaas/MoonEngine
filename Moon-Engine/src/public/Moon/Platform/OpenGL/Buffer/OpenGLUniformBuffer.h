#pragma once

#include "Moon/Core/Renderer/Buffer/UniformBuffer.h"


namespace Moon {

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		RendererID m_RendererID = 0;

	};

}
