#include "Flibbert/Renderer/Buffer.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Backends/OpenGL/OpenGLBuffer.h"

namespace Flibbert {

std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(const float* vertices, const uint32_t size)
{
	switch (Renderer::GetAPI()) {
	case Renderer::API::OpenGL:
		return std::make_shared<OpenGLVertexBuffer>(vertices, size);
	default:
		return nullptr;
	}
}

std::shared_ptr<IIndexBuffer> IIndexBuffer::Create(const uint32_t* indices, const uint32_t size)
{
	switch (Renderer::GetAPI()) {
	case Renderer::API::OpenGL:
		return std::make_shared<OpenGLIndexBuffer>(indices, size);
	default:
		return nullptr;
	}
}

std::shared_ptr<IUniformBuffer> IUniformBuffer::Create(uint32_t size, uint32_t binding)
{
	switch (Renderer::GetAPI()) {
	case Renderer::API::OpenGL:
		return std::make_shared<OpenGLUniformBuffer>(size, binding);
	default:
		return nullptr;
	}
}

} // namespace Flibbert
