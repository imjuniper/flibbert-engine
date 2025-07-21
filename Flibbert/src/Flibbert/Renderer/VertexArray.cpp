#include "Flibbert/Renderer/VertexArray.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/D3D11/D3D11VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Flibbert
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLVertexArray>();
			case Renderer::API::DirectX11:
				return std::make_shared<D3D11VertexArray>();
			default:
				return nullptr;
		}
	}
} // namespace Flibbert
