#include "Platform/D3D11/D3D11Buffer.h"

#include "Flibbert/Renderer/Renderer.h"

#include <d3d11.h>

namespace Flibbert
{
	D3D11VertexBuffer::D3D11VertexBuffer(const void* data, uint32_t size)
	{
		const CD3D11_BUFFER_DESC bufferDesc(size, D3D11_BIND_VERTEX_BUFFER);

		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		bufferData.pSysMem = data;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		const auto device = static_cast<ID3D11Device*>(Renderer::Get().GetDevice());
		device->CreateBuffer(&bufferDesc, &bufferData, &m_VertexBuffer);
	}

	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
		if (m_VertexBuffer) {
			m_VertexBuffer->Release();
		}
	}

	void D3D11VertexBuffer::Bind() const
	{
		const auto context =
		    static_cast<ID3D11DeviceContext*>(Renderer::Get().GetDeviceContext());

		// @todo implement layouts
		constexpr uint32_t stride = 20;
		constexpr uint32_t offset = 0;
		context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	}

	void D3D11VertexBuffer::Unbind() const
	{
		FBT_CORE_WARN("D3D11VertexBuffer::Unbind() is not implemented.");
	}

	D3D11IndexBuffer::D3D11IndexBuffer(const uint32_t* data, const uint32_t count)
	    : m_Count(count)
	{
		const CD3D11_BUFFER_DESC bufferDesc(count * sizeof(uint32_t),
		                                    D3D11_BIND_INDEX_BUFFER);

		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		bufferData.pSysMem = data;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		const auto device = static_cast<ID3D11Device*>(Renderer::Get().GetDevice());
		device->CreateBuffer(&bufferDesc, &bufferData, &m_IndexBuffer);
	}

	D3D11IndexBuffer::~D3D11IndexBuffer()
	{
		if (m_IndexBuffer) {
			m_IndexBuffer->Release();
		}
	}

	void D3D11IndexBuffer::Bind() const
	{
		const auto context =
		    static_cast<ID3D11DeviceContext*>(Renderer::Get().GetDeviceContext());
		context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void D3D11IndexBuffer::Unbind() const
	{
		FBT_CORE_WARN("D3D11IndexBuffer::Unbind() is not implemented.");
	}

	D3D11UniformBuffer::D3D11UniformBuffer(const uint32_t size, const uint32_t binding)
	{
		FBT_CORE_WARN("D3D11UniformBuffer's binding is not used.");
		const CD3D11_BUFFER_DESC bufferDesc(size, D3D11_BIND_CONSTANT_BUFFER);

		const auto device = static_cast<ID3D11Device*>(Renderer::Get().GetDevice());
		device->CreateBuffer(&bufferDesc, nullptr, &m_ConstantBuffer);
	}

	D3D11UniformBuffer::~D3D11UniformBuffer()
	{
		if (m_ConstantBuffer) {
			m_ConstantBuffer->Release();
		}
	}

	void D3D11UniformBuffer::SetData(const void* data, const uint32_t size,
	                                 const uint32_t offset)
	{
		const auto context =
		    static_cast<ID3D11DeviceContext*>(Renderer::Get().GetDeviceContext());
		context->UpdateSubresource(m_ConstantBuffer, 0, nullptr, data, 0, 0);
	}
} // namespace Flibbert
