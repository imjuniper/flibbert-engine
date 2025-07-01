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

		void AddBuffer(const VertexBuffer& vertexBuffer) const override;

	private:
		uint32_t m_RendererID;
	};
} // namespace Flibbert
