#include "Flibbert/Renderer/Renderer.h"

#include "Flibbert/Core/Application.h"
#include "Platform/OpenGL/OpenGLRendererBackend.h"

namespace Flibbert
{
	Renderer::API Renderer::s_API = API::OpenGL;

	Renderer::Renderer()
	{
		switch (s_API) {
			case API::OpenGL:
				m_Backend = std::make_unique<OpenGLRendererBackend>();
				break;
			default:
				FBT_CORE_CRITICAL("Unsupported or unknown Renderer::API");
				assert(false);
		}
	}

	RendererBackend& Renderer::GetBackend() const
	{
		return *m_Backend;
	}

	Renderer& Renderer::Get()
	{
		return Application::Get().GetRenderer();
	}

	void Renderer::InitGraphicsContext(void* window)
	{
		switch (s_API) {
			case API::OpenGL:
				OpenGLRendererBackend::InitGraphicsContext(
				    static_cast<RGFW_window*>(window));
				break;
			default:
				FBT_CORE_CRITICAL("Unsupported or unknown Renderer::API");
				assert(false);
		}
	}
} // namespace Flibbert
