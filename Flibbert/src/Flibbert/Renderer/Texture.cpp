#include "Flibbert/Renderer/Texture.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Flibbert
{
	Texture* Texture::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return new OpenGLTexture(path);
			case Renderer::API::None:
				// assert(false, "Renderer::API::None is currently not supported!");
				break;
		}

		// assert(false, "Unknown Renderer::API!");
		return nullptr;
	}
} // namespace Flibbert
