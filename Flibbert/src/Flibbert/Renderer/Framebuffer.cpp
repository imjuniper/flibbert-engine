#include "Flibbert/Renderer/Framebuffer.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Backends/OpenGL/OpenGLFramebuffer.h"

namespace Flibbert {

std::shared_ptr<IFramebuffer> IFramebuffer::Create(const FramebufferSpecification& specification)
{
	switch (Renderer::GetAPI()) {
	case Renderer::API::OpenGL:
		return std::make_shared<OpenGLFramebuffer>(specification);
	default:
		return nullptr;
	}
}

} // namespace Flibbert
