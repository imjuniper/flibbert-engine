#include "Platform/Desktop/Window.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Renderer/Renderer.h"

#define RGFW_IMPLEMENTATION
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#include <external/RGFW.h>
#pragma clang diagnostic pop

namespace Flibbert
{
	Window::Window(const WindowProps& props)
	{
		// @todo check if this is needed on other platforms than macOS
		if (Application::Get().GetRenderer()->GetAPI() == Renderer::API::OpenGL) {
			RGFW_setGLHint(RGFW_glMajor, 4);
#ifdef FBT_PLATFORM_MACOS
			/* Set OpenGL version to 4.1 on macOS */
			RGFW_setGLHint(RGFW_glMinor, 1);
#else
			RGFW_setGLHint(RGFW_glMinor, 6);
#endif
			RGFW_setGLHint(RGFW_glProfile, RGFW_glCore);
		}

		m_WindowHandle = RGFW_createWindow(props.Title.c_str(),
		                                   RGFW_RECT(0, 0, props.Width, props.Height),
		                                   RGFW_windowCenter);

		Renderer::InitGraphicsContext(m_WindowHandle);
	}

	Window::~Window()
	{
		RGFW_window_close(m_WindowHandle);
	}
} // namespace Flibbert
