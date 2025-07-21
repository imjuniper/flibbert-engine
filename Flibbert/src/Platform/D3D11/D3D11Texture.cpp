#include "Platform/D3D11/D3D11Texture.h"

#include <stb_image.h>

namespace Flibbert
{
	D3D11Texture::D3D11Texture(std::string_view path)
	{
		FBT_PROFILE_FUNCTION();

		FBT_CORE_WARN("D3D11Texture not implemented");

		stbi_set_flip_vertically_on_load(1);
		int width, height, channels;
		void* data = nullptr;
		{
			FBT_PROFILE_SCOPE(
			    "stbi_load - D3D11Texture::D3D11Texture(std::string_view)");
			data = stbi_load(path.data(), &width, &height, &channels, 0);
		}
		if (data == nullptr) return;

		stbi_image_free(data);
	}

	D3D11Texture::~D3D11Texture()
	{
	}

	void D3D11Texture::Bind(uint32_t slot) const
	{
	}

	void D3D11Texture::Unbind() const
	{
	}
} // namespace Flibbert
