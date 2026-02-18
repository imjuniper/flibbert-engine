#pragma once

#include "Flibbert/Renderer/Texture.h"

namespace Flibbert {

class OpenGLTexture : public ITexture
{
public:
	explicit OpenGLTexture(std::string_view path);
	~OpenGLTexture() override;

	virtual void MakeResident() override;
	virtual void MakeNonResident() override;

private:
	uint32_t m_RendererID;
	bool m_Resident = false;
};

} // namespace Flibbert
