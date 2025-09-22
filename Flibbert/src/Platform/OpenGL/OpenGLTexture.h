#pragma once

#include "Flibbert/Renderer/Texture.h"

#include <string>

namespace Flibbert
{
	class OpenGLTexture : public Texture
	{
	public:
		explicit OpenGLTexture(std::string_view path);
		~OpenGLTexture() override;

		void Bind(uint32_t slot) const override;
		void Unbind(uint32_t slot) const override;

	private:
		uint32_t m_RendererID;
	};
} // namespace Flibbert
