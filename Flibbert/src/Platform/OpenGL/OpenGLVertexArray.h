#pragma once

namespace Flibbert
{
	class OpenGLVertexBuffer;
	class VertexBufferLayout;

	class OpenGLVertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void AddBuffer(const OpenGLVertexBuffer& vb,
		               const VertexBufferLayout& layout) const;
		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_RendererID;
	};
} // namespace Flibbert
