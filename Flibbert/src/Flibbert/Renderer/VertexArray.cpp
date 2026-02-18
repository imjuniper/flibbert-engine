#include "Flibbert/Renderer/VertexArray.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Backends/OpenGL/OpenGLVertexArray.h"

namespace Flibbert {

std::shared_ptr<IVertexArray> IVertexArray::Create()
{
	switch (Renderer::GetAPI()) {
	case Renderer::API::OpenGL:
		return std::make_shared<OpenGLVertexArray>();
	default:
		return nullptr;
	}
}

} // namespace Flibbert
