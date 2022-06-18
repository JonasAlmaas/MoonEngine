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

		virtual RendererID GetColorAttachmentRendererID() const override { return m_ColorAttachmentRendererID; };
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; };

	private:
		RendererID m_RendererID;
		RendererID m_ColorAttachmentRendererID;
		RendererID m_DepthAttachmentRendererID;
		FramebufferSpecification m_Specification;

	};

}
