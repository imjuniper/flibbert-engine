#pragma once

#include "Flibbert/Renderer/Shader.h"

#include <string>
#include <unordered_map>

namespace Flibbert
{
	class D3D11Shader : public Shader
	{
	public:
		D3D11Shader(std::string_view vertexShaderFilepath,
		             std::string_view fragmentShaderFilepath);
		~D3D11Shader() override;

		void Bind() const override;
		void Unbind() const override;

		// Set uniforms
		void SetUniform1i(std::string_view name, int value) override;
		void SetUniform1f(std::string_view name, float value) override;
		void SetUniform2f(std::string_view name, const glm::vec2& value) override;
		void SetUniform4f(std::string_view name, const glm::vec4& value) override;
		void SetUniformMat4f(std::string_view name, const glm::mat4& value) override;

		void BindUniformBlock(std::string_view name, uint32_t binding) override;

	private:
		std::string m_VertexShaderFilePath;
		std::string m_FragmentShaderFilePath;
	};
} // namespace Flibbert
