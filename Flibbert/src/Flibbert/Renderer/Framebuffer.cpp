#include "Flibbert/Renderer/Framebuffer.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Flibbert {
	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLFramebuffer>(specification);
			default:
				return nullptr;
		}
	}
}
