#pragma once

#include "Flibbert/Renderer/Texture.h"

#include <string>

namespace Flibbert
{
	class D3D11Texture : public Texture
	{
	public:
		explicit D3D11Texture(std::string_view path);
		~D3D11Texture() override;

		void Bind(uint32_t slot) const override;
		void Unbind() const override;
	};
} // namespace Flibbert
