#include "Flibbert/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLRendererBackend.h"

namespace Flibbert
{
	Renderer::API Renderer::s_API = API::OpenGL;

	Renderer::Renderer()
	{
		switch (s_API) {
			case API::OpenGL:
				m_Backend = new OpenGLRendererBackend();
			case API::None:
				// HZ_CORE_ASSERT(false, "Renderer::API::None is currently not
				// supported!");
				break;
		}
	}

	void Renderer::InitGraphicsContext(void* window)
	{
		switch (s_API) {
			case API::OpenGL:
				OpenGLRendererBackend::InitGraphicsContext(
				    static_cast<RGFW_window*>(window));
			case API::None:
				// HZ_CORE_ASSERT(false, "Renderer::API::None is currently not
				// supported!");
				break;
		}
	}
} // namespace Flibbert
