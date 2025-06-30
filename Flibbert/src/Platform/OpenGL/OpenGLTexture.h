#pragma once

#include <external/glad.h>

#include <string>

namespace Flibbert
{
	class OpenGLTexture
	{
	public:
		explicit OpenGLTexture(const std::string& path);
		~OpenGLTexture();

		void Bind(uint32_t slot = 0) const;
		void Unbind() const;

		[[nodiscard]] uint32_t GetWidth() const { return m_Width; }
		[[nodiscard]] uint32_t GetHeight() const { return m_Height; }

	private:
		std::string m_Path;
		bool m_Loaded = false;

		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		GLenum m_InternalFormat, m_DataFormat;
	};
} // namespace Flibbert
