#include "Flibbert/Renderer/Texture.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/D3D11/D3D11Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Flibbert
{
	std::shared_ptr<Texture> Texture::Create(std::string_view path)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLTexture>(path);
			case Renderer::API::DirectX11:
				return std::make_shared<D3D11Texture>(path);
			default:
				return nullptr;
		}
	}
} // namespace Flibbert
