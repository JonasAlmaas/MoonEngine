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

		virtual RendererID GetColorAttachmentRendererID(uint32_t index = 0) const override
		{
			ME_CORE_ASSERT(index < m_ColorAttachments.size(), "");
			return m_ColorAttachments[index];
		};
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; };

	private:
		RendererID m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification;

		std::vector<RendererID> m_ColorAttachments;
		RendererID m_DepthAttachment;

	};

}
