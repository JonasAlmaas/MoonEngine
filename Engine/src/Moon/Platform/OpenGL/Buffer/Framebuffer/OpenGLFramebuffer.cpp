#include "mepch.h"
#include "Moon/Platform/OpenGL/Buffer/Framebuffer/OpenGLFramebuffer.h"

#include <glad/glad.h>


namespace Moon {

	static const uint32_t s_MaxFrameBufferSize = 16384;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		ME_PROFILE_FUNCTION();

		ME_CORE_ASSERT(m_Specification.Width, "Frambuffers can not be created with a width of 0!");
		ME_CORE_ASSERT(m_Specification.Width, "Frambuffers can not be created with a height of 0!");

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		ME_PROFILE_FUNCTION();

		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachmentRendererID);
		glDeleteTextures(1, &m_DepthAttachmentRendererID);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		ME_PROFILE_FUNCTION();

		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachmentRendererID);
			glDeleteTextures(1, &m_DepthAttachmentRendererID);
		}

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
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		ME_PROFILE_FUNCTION();

		// Check if the buffer is an invalid size
		if (width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
		{
			ME_CORE_WARN("Attempted to resize frame buffer to {0}, {1}", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

}
