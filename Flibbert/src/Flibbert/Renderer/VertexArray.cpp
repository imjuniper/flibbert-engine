#include "Flibbert/Renderer/VertexArray.h"

#include "Flibbert/Renderer/Renderer.h"
#ifndef FBT_PLATFORM_MACOS
	#include "Platform/OpenGL/OpenGLVertexArray.h"
#endif

namespace Flibbert
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
#ifndef FBT_PLATFORM_MACOS
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLVertexArray>();
#endif
			default:
				return nullptr;
		}
	}
} // namespace Flibbert
