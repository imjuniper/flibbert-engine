#pragma once

#include "Flibbert/Renderer/Shader.h"

#include <filesystem>

namespace Flibbert
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::string_view vertexShaderFilepath,
		             std::string_view fragmentShaderFilepath);
		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		// Set uniforms
		[[deprecated]] void SetUniform1i(std::string_view name, int value) override;

		void BindUniformBuffer(std::string_view name, uint32_t binding) override;

	private:
		uint32_t CompileShader(uint32_t type, const std::string& source);
		uint32_t CreateShader(const std::string& vertexShader,
		                      const std::string& fragmentShader);
		int GetUniformLocation(std::string_view name);

	private:
		std::filesystem::path m_VertexShaderFilePath;
		std::filesystem::path m_FragmentShaderFilePath;
		uint32_t m_RendererID;
	};
} // namespace Flibbert
