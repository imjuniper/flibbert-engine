#include "Flibbert/Renderer/Buffer.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/D3D11/D3D11Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Flibbert
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const float* vertices,
	                                                   const uint32_t size)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLVertexBuffer>(vertices, size);
			case Renderer::API::DirectX11:
				return std::make_shared<D3D11VertexBuffer>(vertices, size);
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
			case Renderer::API::DirectX11:
				return std::make_shared<D3D11IndexBuffer>(indices, size);
			default:
				return nullptr;
		}
	}

	std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLUniformBuffer>(size, binding);
			case Renderer::API::DirectX11:
				return std::make_shared<D3D11UniformBuffer>(size, binding);
			default:
				return nullptr;
		}
	}
} // namespace Flibbert
