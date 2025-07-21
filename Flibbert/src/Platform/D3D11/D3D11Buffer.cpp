#include "Platform/D3D11/D3D11Buffer.h"

namespace Flibbert
{
	D3D11VertexBuffer::D3D11VertexBuffer(const void* data, uint32_t size)
	{
		FBT_CORE_WARN("D3D11VertexBuffer not implemented");
	}

	D3D11VertexBuffer::~D3D11VertexBuffer() {}

	void D3D11VertexBuffer::Bind() const {}

	void D3D11VertexBuffer::Unbind() const {}

	D3D11IndexBuffer::D3D11IndexBuffer(const uint32_t* data, const uint32_t count)
	    : m_Count(count)
	{
		FBT_CORE_WARN("D3D11IndexBuffer not implemented");
	}

	D3D11IndexBuffer::~D3D11IndexBuffer() {}

	void D3D11IndexBuffer::Bind() const {}

	void D3D11IndexBuffer::Unbind() const {}

	D3D11UniformBuffer::D3D11UniformBuffer(const uint32_t size, const uint32_t binding)
	{
		FBT_CORE_WARN("D3D11UniformBuffer not implemented");
	}

	D3D11UniformBuffer::~D3D11UniformBuffer() {}

	void D3D11UniformBuffer::SetData(const void* data, const uint32_t size,
	                                 const uint32_t offset)
	{
	}
} // namespace Flibbert
