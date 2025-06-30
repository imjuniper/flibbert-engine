#include "Window.h"

#include "OpenGL/Renderer.h"

#define GLAD_GL_IMPLEMENTATION
#include <external/glad.h>

#define RGFW_IMPLEMENTATION
#include <external/RGFW.h>

namespace Flibbert
{
	Window::Window()
	{
		RGFW_setGLHint(RGFW_glMajor, 4);
#ifdef FBT_PLATFORM_MACOS
		/* Set OpenGL version to 4.1 on macOS */
		RGFW_setGLHint(RGFW_glMinor, 1);
#else
		RGFW_setGLHint(RGFW_glMinor, 6);
#endif
		RGFW_setGLHint(RGFW_glProfile, RGFW_glCore);

		/* Create a windowed mode window and its src context */
		m_Window = RGFW_createWindow("OpenGL Learning", RGFW_RECT(0, 0, 960, 540),
		                             RGFW_windowCenter);
		if (!m_Window) return;

		RGFW_window_makeCurrent(m_Window);

		// @todo move this to Renderer class
		int status = gladLoadGL((GLADloadfunc)RGFW_getProcAddress);
		assert(status);

		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	Window::~Window()
	{
		RGFW_window_close(m_Window);
	}
} // namespace Flibbert
