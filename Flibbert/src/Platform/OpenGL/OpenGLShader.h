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
		void SetUniform1i(std::string_view name, int value) override;
		void SetUniform1f(std::string_view name, float value) override;
		void SetUniform2f(std::string_view name, const glm::vec2& value) override;
		void SetUniform3f(std::string_view name, const glm::vec3& value) override;
		void SetUniform4f(std::string_view name, const glm::vec4& value) override;
		void SetUniformMat4f(std::string_view name, const glm::mat4& value) override;

		void BindUniformBlock(std::string_view name, uint32_t binding) override;

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
