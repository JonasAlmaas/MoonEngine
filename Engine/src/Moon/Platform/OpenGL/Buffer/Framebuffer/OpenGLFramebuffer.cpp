#include "mepch.h"
#include "Moon/Platform/OpenGL/Buffer/Framebuffer/OpenGLFramebuffer.h"

#include <glad/glad.h>


namespace Moon {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		ME_PROFILE_FUNCTION();

		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		ME_PROFILE_FUNCTION();

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		// -- Color Attachment --
		glCreateTextures(GL_TEXTURE_2D ,1, &m_ColorAttachmentRendererID);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentRendererID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentRendererID, 0);

		// -- Depth Attachment --
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachmentRendererID);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachmentRendererID);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachmentRendererID, 0);

		// Check frame buffer status
		ME_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
