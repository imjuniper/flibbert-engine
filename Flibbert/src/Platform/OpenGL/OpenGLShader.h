#pragma once

#include "Flibbert/Renderer/Shader.h"

#include <string>
#include <unordered_map>

namespace Flibbert
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexShaderFilepath,
		             const std::string& fragmentShaderFilepath);
		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		// Set uniforms
		void SetUniform1i(const std::string& name, int value) override;
		void SetUniform1f(const std::string& name, float value) override;
		void SetUniform2f(const std::string& name, const glm::vec2& value) override;
		void SetUniform4f(const std::string& name, const glm::vec4& value) override;
		void SetUniformMat4f(const std::string& name, const glm::mat4& value) override;

		void BindUniformBlock(const std::string& name, uint32_t binding) override;

	private:
		std::string LoadShader(const std::string& filepath);
		uint32_t CompileShader(uint32_t type, const std::string& source);
		uint32_t CreateShader(const std::string& vertexShader,
		                      const std::string& fragmentShader);
		int GetUniformLocation(const std::string& name);

	private:
		std::string m_VertexShaderFilePath;
		std::string m_FragmentShaderFilePath;
		uint32_t m_RendererID;
		std::unordered_map<std::string, int> m_UniformLocationCache;
		// caching for uniforms
	};
} // namespace Flibbert
