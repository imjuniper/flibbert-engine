#include "Platform/D3D11/D3D11VertexArray.h"

#include "Platform/D3D11/D3D11Buffer.h"

namespace Flibbert
{
	void D3D11VertexArray::Bind() const
	{
		for (const auto& vertexBuffer : m_VertexBuffers) {
			vertexBuffer->Bind();
		}
		m_IndexBuffer->Bind();
	}

	void D3D11VertexArray::Unbind() const
	{
		for (const auto& vertexBuffer : m_VertexBuffers) {
			vertexBuffer->Unbind();
		}
		m_IndexBuffer->Unbind();
	}

	void D3D11VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		m_VertexBuffers.push_back(vertexBuffer);
	}

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
