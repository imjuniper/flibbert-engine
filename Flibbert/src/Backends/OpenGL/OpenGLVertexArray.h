#pragma once

#include "Flibbert/Renderer/VertexArray.h"

namespace Flibbert {

class OpenGLVertexArray : public IVertexArray
{
public:
	OpenGLVertexArray();
	~OpenGLVertexArray() override;

	void Bind() const override;
	void Unbind() const override;

	void AddVertexBuffer(std::shared_ptr<IVertexBuffer> vertexBuffer) override;
	void SetIndexBuffer(std::shared_ptr<IIndexBuffer> indexBuffer) override;

	const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffers() const override;
	const std::shared_ptr<IIndexBuffer>& GetIndexBuffer() const override;

private:
	uint32_t m_RendererID;
	uint32_t m_VertexAttributeIndex = 0;
	std::vector<std::shared_ptr<IVertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IIndexBuffer> m_IndexBuffer;
};

} // namespace Flibbert
