#include "Flibbert/Renderer/Texture.h"

#include "Flibbert/Renderer/Renderer.h"
#ifndef FBT_PLATFORM_MACOS
	#include "Platform/OpenGL/OpenGLTexture.h"
#endif

namespace Flibbert
{
	std::shared_ptr<Texture> Texture::Create(std::string_view path)
	{
		switch (Renderer::GetAPI()) {
#ifndef FBT_PLATFORM_MACOS
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLTexture>(path);
#endif
			default:
				return nullptr;
		}
	}
} // namespace Flibbert
