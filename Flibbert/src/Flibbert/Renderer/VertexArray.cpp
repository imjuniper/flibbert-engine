#include "VertexArray.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Flibbert
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return new OpenGLVertexArray();
			case Renderer::API::None:
				// assert(false, "Renderer::API::None is currently not supported!");
				break;
		}

		// assert(false, "Unknown Renderer::API!");
		return nullptr;
	}
} // namespace Flibbert
