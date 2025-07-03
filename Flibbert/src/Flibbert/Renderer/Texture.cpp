#include "Flibbert/Renderer/Texture.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Flibbert
{
	std::shared_ptr<Texture> Texture::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLTexture>(path);
			default:
				return nullptr;
		}
	}
} // namespace Flibbert
