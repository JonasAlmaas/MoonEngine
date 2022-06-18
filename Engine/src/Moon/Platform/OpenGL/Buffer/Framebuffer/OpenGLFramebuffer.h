#pragma once

#include "Moon/Core/Renderer/Buffer/Framebuffer/Framebuffer.h"


namespace Moon {
	
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		/**
		 * @brief Recreate framebuffer
		 */
		void Invalidate();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual RendererID GetColorAttachmentRendererID() const override { return m_ColorAttachmentRendererID; };
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; };

	private:
		RendererID m_RendererID = 0;
		RendererID m_ColorAttachmentRendererID = 0;
		RendererID m_DepthAttachmentRendererID = 0;
		FramebufferSpecification m_Specification;

	};

}
