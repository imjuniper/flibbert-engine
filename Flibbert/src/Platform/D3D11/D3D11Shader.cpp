#include "Platform/D3D11/D3D11Shader.h"

#include "Flibbert/Renderer/Renderer.h"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace Flibbert
{
	D3D11Shader::D3D11Shader(const std::string_view vertexShaderFilepath,
	                         const std::string_view fragmentShaderFilepath)
	    : m_VertexShaderFilePath(vertexShaderFilepath),
	      m_FragmentShaderFilePath(fragmentShaderFilepath)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if !defined(NDEBUG)
		flags |= D3DCOMPILE_DEBUG;
		flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		const auto device = static_cast<ID3D11Device*>(Renderer::Get().GetDevice());

		m_VertexShaderFilePath.append(".hlsl");
		m_FragmentShaderFilePath.append(".hlsl");

		{
			ID3DBlob* errorBlob = nullptr;

			std::ifstream stream(m_VertexShaderFilePath.data());
			std::stringstream ss;
			ss << stream.rdbuf();
			const auto code = ss.str();

			auto hr = D3DCompile(code.data(), code.size(), nullptr, nullptr,
			           nullptr, "VSMain", "vs_5_0", flags, 0,
			           &m_VertexShaderBlob, &errorBlob);

			if (FAILED(hr)) {

				std::cerr << "Shader compilation failed: "

					  << (errorBlob ? (char*)errorBlob->GetBufferPointer() : "Unknown error") << std::endl;

				exit(-1);

			}

			device->CreateVertexShader(m_VertexShaderBlob->GetBufferPointer(),
			                           m_VertexShaderBlob->GetBufferSize(), nullptr,
			                           &m_VertexShader);
		}

		{
			ID3DBlob* shaderBlob = nullptr;
			ID3DBlob* errorBlob = nullptr;

			std::ifstream stream(m_FragmentShaderFilePath.data());
			std::stringstream ss;
			ss << stream.rdbuf();
			const auto code = ss.str();

			D3DCompile(code.data(), code.size(), nullptr, nullptr,
			           nullptr, "PSMain", "ps_5_0", flags, 0,
			           &shaderBlob, &errorBlob);

			device->CreatePixelShader(shaderBlob->GetBufferPointer(),
			                          shaderBlob->GetBufferSize(), nullptr,
			                          &m_PixelShader);
		}
	}

	D3D11Shader::~D3D11Shader()
	{
		if (m_VertexShader) {
			m_VertexShader->Release();
		}
		if (m_PixelShader) {
			m_PixelShader->Release();
		}
	}

	void D3D11Shader::Bind() const
	{
		const auto context =
		    static_cast<ID3D11DeviceContext*>(Renderer::Get().GetDeviceContext());

		context->VSSetShader(m_VertexShader, nullptr, 0);
		context->PSSetShader(m_PixelShader, nullptr, 0);
	}

	void D3D11Shader::Unbind() const {}

	void D3D11Shader::SetUniform1i(std::string_view name, const int value) {}

	void D3D11Shader::SetUniform1f(std::string_view name, const float value) {}

	void D3D11Shader::SetUniform2f(std::string_view name, const glm::vec2& value) {}

	void D3D11Shader::SetUniform4f(std::string_view name, const glm::vec4& value) {}

	void D3D11Shader::SetUniformMat4f(std::string_view name, const glm::mat4& value) {}

	void D3D11Shader::BindUniformBuffer(std::string_view name, uint32_t binding) {}
} // namespace Flibbert
