#pragma once

#include "Flibbert/Core/Base.h"

#include "Flibbert/Renderer/Framebuffer.h"

namespace Flibbert
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		~OpenGLFramebuffer() override;

		void Bind() override;
		void Unbind() override;

		void Resize(uint32_t width, uint32_t height) override;
		void Clear() override;
		uint32_t GetColorBufferID() override { return m_ColorBufferID; }

	private:
		void Invalidate();

	private:
		FramebufferSpecification m_Specification;
		uint32_t m_RendererID = 0;
		uint32_t m_ColorBufferID = 0;
		uint32_t m_DepthBufferID = 0;
	};
}
