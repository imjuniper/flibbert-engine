#include "Platform/D3D11/D3D11Shader.h"

namespace Flibbert
{
	D3D11Shader::D3D11Shader(std::string_view vertexShaderFilepath,
	                         std::string_view fragmentShaderFilepath)
	    : m_VertexShaderFilePath(vertexShaderFilepath),
	      m_FragmentShaderFilePath(fragmentShaderFilepath)
	{
		FBT_CORE_WARN("D3D11Shader not implemented");
	}

	D3D11Shader::~D3D11Shader() {}

	void D3D11Shader::Bind() const {}

	void D3D11Shader::Unbind() const {}

	void D3D11Shader::SetUniform1i(std::string_view name, const int value) {}

	void D3D11Shader::SetUniform1f(std::string_view name, const float value) {}

	void D3D11Shader::SetUniform2f(std::string_view name, const glm::vec2& value) {}

	void D3D11Shader::SetUniform4f(std::string_view name, const glm::vec4& value) {}

	void D3D11Shader::SetUniformMat4f(std::string_view name, const glm::mat4& value) {}

	void D3D11Shader::BindUniformBlock(std::string_view name, uint32_t binding) {}
} // namespace Flibbert
