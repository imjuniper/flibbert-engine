#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Flibbert
{
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
	    : m_Specification(specification)
	{
		ZoneScoped;

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		ZoneScoped;

		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFramebuffer::Bind()
	{
		ZoneScoped;

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		ZoneScoped;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		ZoneScoped;

		if (width <= 0 || height <= 0) {
			FBT_CORE_WARN("Tried to resize a framebuffer to an invalid size: {0}x{0}", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	void OpenGLFramebuffer::Clear()
	{
		ZoneScoped;

		// @todo figure out an API for this
		static constexpr glm::vec4 clearColor(0.0f, 0.0f, 0.0f, 1.0f);
		static constexpr glm::mat4 clearDepth(1.0f);

		glClearNamedFramebufferfv(m_RendererID, GL_COLOR, 0, glm::value_ptr(clearColor));
		glClearNamedFramebufferfv(m_RendererID, GL_DEPTH, 0, glm::value_ptr(clearDepth));
	}

	void OpenGLFramebuffer::Invalidate()
	{
		ZoneScoped;

		// Create the framebuffer and its textures/render buffers
		if (m_RendererID == 0) {
			glCreateFramebuffers(1, &m_RendererID);

			glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorBufferID);
			glTextureParameteri(m_ColorBufferID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_ColorBufferID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0, m_ColorBufferID, 0);

			glCreateRenderbuffers(1, &m_DepthBufferID);
			glNamedFramebufferRenderbuffer(m_RendererID, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
			                               m_DepthBufferID);
		}

		// Set their size
		glTextureStorage2D(m_ColorBufferID, 1, GL_RGB8, m_Specification.Width, m_Specification.Height);
		glNamedRenderbufferStorage(m_DepthBufferID, GL_DEPTH24_STENCIL8, m_Specification.Width,
		                           m_Specification.Height);

		FBT_CORE_ENSURE_MSG(glCheckNamedFramebufferStatus(m_RendererID, GL_FRAMEBUFFER) ==
		                        GL_FRAMEBUFFER_COMPLETE,
		                    "Framebuffer is incomplete!");
	}
} // namespace Flibbert
