#include "Flibbert/Renderer/Buffer.h"

#include "Flibbert/Renderer/Renderer.h"
#ifndef FBT_PLATFORM_MACOS
	#include "Platform/OpenGL/OpenGLBuffer.h"
#endif

namespace Flibbert
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const float* vertices,
	                                                   const uint32_t size)
	{
		switch (Renderer::GetAPI()) {
#ifndef FBT_PLATFORM_MACOS
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLVertexBuffer>(vertices, size);
#endif
			default:
				return nullptr;
		}
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(const uint32_t* indices,
	                                                 const uint32_t size)
	{
		switch (Renderer::GetAPI()) {
#ifndef FBT_PLATFORM_MACOS
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLIndexBuffer>(indices, size);
#endif
			default:
				return nullptr;
		}
	}

	std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI()) {
#ifndef FBT_PLATFORM_MACOS
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLUniformBuffer>(size, binding);
#endif
			default:
				return nullptr;
		}
	}
} // namespace Flibbert
