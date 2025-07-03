#include "Flibbert/Renderer/Buffer.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Flibbert
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const float* vertices,
	                                                   const uint32_t size)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLVertexBuffer>(vertices, size);
			default:
				return nullptr;
		}
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(const uint32_t* indices,
	                                                 const uint32_t size)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLIndexBuffer>(indices, size);
			default:
				return nullptr;
		}
	}
} // namespace Flibbert
