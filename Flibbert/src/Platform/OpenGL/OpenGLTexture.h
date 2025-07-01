#pragma once

#include "Flibbert/Renderer/Texture.h"

#include <string>

namespace Flibbert
{
	class OpenGLTexture : public Texture
	{
	public:
		explicit OpenGLTexture(const std::string& path);
		~OpenGLTexture() override;

		void Bind(uint32_t slot) const override;
		void Unbind() const override;

	private:
		uint32_t m_RendererID;
	};
} // namespace Flibbert
