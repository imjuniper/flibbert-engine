#include "Flibbert/Renderer/Texture.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Backends/OpenGL/OpenGLTexture.h"

namespace Flibbert {

std::shared_ptr<ITexture> ITexture::Create(std::string_view path)
{
	switch (Renderer::GetAPI()) {
	case Renderer::API::OpenGL:
		return std::make_shared<OpenGLTexture>(path);
	default:
		return nullptr;
	}
}

} // namespace Flibbert
