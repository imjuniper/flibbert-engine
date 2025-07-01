#include "Flibbert/Renderer/Buffer.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Flibbert
{
	VertexBuffer* VertexBuffer::Create(const float* vertices, const uint32_t size)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
			case Renderer::API::None:
				// assert(false, "Renderer::API::None is currently not supported!");
				break;
		}

		// assert(false, "Unknown Renderer::API!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(const uint32_t* indices, const uint32_t size)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return new OpenGLIndexBuffer(indices, size);
			case Renderer::API::None:
				// assert(false, "Renderer::API::None is currently not supported!");
				break;
		}

		// assert(false, "Unknown Renderer::API!");
		return nullptr;
	}
} // namespace Flibbert
