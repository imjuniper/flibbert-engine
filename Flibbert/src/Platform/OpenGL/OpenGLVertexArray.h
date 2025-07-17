#pragma once

#include "Flibbert/Renderer/VertexArray.h"

namespace Flibbert
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;

	private:
		uint32_t m_RendererID;
		u_int32_t m_VertexBufferIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
} // namespace Flibbert
