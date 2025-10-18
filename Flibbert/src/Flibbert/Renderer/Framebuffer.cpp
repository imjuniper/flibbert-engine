#include "Flibbert/Renderer/Framebuffer.h"

#include "Flibbert/Renderer/Renderer.h"
#ifndef FBT_PLATFORM_MACOS
	#include "Platform/OpenGL/OpenGLFramebuffer.h"
#endif

namespace Flibbert {
	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification)
	{
		switch (Renderer::GetAPI()) {
#ifndef FBT_PLATFORM_MACOS
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLFramebuffer>(specification);
#endif
			default:
				return nullptr;
		}
	}
}
