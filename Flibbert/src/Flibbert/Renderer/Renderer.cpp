#include "Flibbert/Renderer/Renderer.h"

#include "Platform/Metal/MetalRendererBackend.h"
#include "Platform/OpenGL/OpenGLRendererBackend.h"

namespace Flibbert
{
	Renderer::API Renderer::s_API = API::Metal;

	Renderer::Renderer(void* window)
	{
		switch (s_API) {
			case API::Metal:
				m_Backend =
				    new MetalRendererBackend(static_cast<RGFW_window*>(window));
				break;
			case API::OpenGL:
				m_Backend =
				    new OpenGLRendererBackend(static_cast<RGFW_window*>(window));
				break;
			case API::None:
			default:
				FBT_CORE_CRITICAL("Unsupported Renderer::API");
				// assert(false, "Renderer::API::None is currently not supported!");
				break;
		}

		// assert(false, "Unknown Renderer::API!");
	}

	void Renderer::InitGraphicsContext(void* window)
	{
		switch (s_API) {
			case API::Metal:
				break;
			case API::OpenGL:
				OpenGLRendererBackend::InitGraphicsContext(
				    static_cast<RGFW_window*>(window));
				break;
			case API::None:
			default:
				FBT_CORE_CRITICAL("Unsupported Renderer::API");
				// assert(false, "Renderer::API::None is currently not supported!");
				break;
		}

		// assert(false, "Unknown Renderer::API!");
	}
} // namespace Flibbert
