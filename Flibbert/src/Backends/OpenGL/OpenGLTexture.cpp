#include "Backends/OpenGL/OpenGLTexture.h"

#include <glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Flibbert {

OpenGLTexture::OpenGLTexture(std::string_view path) : m_RendererID(0)
{
	FBT_PROFILE_FUNCTION();

	stbi_set_flip_vertically_on_load(1);
	int width, height, channels;
	void* data = nullptr;
	{
		FBT_PROFILE_SCOPE("stbi_load(...)");
		data = stbi_load(path.data(), &width, &height, &channels, 0);
	}
	if (data == nullptr)
		return;

	m_Loaded = true;
	m_Width = width;
	m_Height = height;

	GLenum internalFormat = 0, dataFormat = 0;
	if (channels == 4) {
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels == 3) {
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}
	m_InternalFormat = internalFormat;
	m_DataFormat = dataFormat;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

	glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	m_BindlessHandle = glGetTextureHandleARB(m_RendererID);

	stbi_image_free(data);
}

OpenGLTexture::~OpenGLTexture()
{
	FBT_PROFILE_FUNCTION();

	glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture::MakeResident()
{
	FBT_PROFILE_FUNCTION();

	if (!m_Resident) {
		glMakeTextureHandleResidentARB(m_BindlessHandle);
		m_Resident = true;
	}
}

void OpenGLTexture::MakeNonResident()
{
	FBT_PROFILE_FUNCTION();

	if (m_Resident) {
		glMakeTextureHandleNonResidentARB(m_BindlessHandle);
		m_Resident = false;
	}
}

} // namespace Flibbert
