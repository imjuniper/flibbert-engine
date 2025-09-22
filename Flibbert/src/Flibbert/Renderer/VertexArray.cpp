#include "Flibbert/Renderer/VertexArray.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Flibbert
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLVertexArray>();
			default:
				return nullptr;
		}
	}
} // namespace Flibbert
