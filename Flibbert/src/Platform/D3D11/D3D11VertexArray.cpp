#include "Platform/D3D11/D3D11VertexArray.h"

#include "Platform/D3D11/D3D11Buffer.h"

namespace Flibbert
{
	D3D11VertexArray::D3D11VertexArray()
	{
		FBT_CORE_WARN("D3D11VertexArray not implemented");
	}

	D3D11VertexArray::~D3D11VertexArray() {}

	void D3D11VertexArray::Bind() const {}

	void D3D11VertexArray::Unbind() const {}

	void D3D11VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) {}

	const std::vector<std::shared_ptr<VertexBuffer>>& D3D11VertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& D3D11VertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

	void D3D11VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
	{
		m_IndexBuffer = indexBuffer;
	}
} // namespace Flibbert
