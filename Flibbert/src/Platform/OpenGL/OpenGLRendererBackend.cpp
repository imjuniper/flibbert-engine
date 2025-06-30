#include "Platform/OpenGL/OpenGLRendererBackend.h"

#include <external/RGFW.h>

#define GLAD_GL_IMPLEMENTATION
#include <external/glad.h>

/*
void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity,
                           int length, const char* message, const void* userParam)
{
        switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:
                        FBT_CORE_CRITICAL(message);
                        return;
                case GL_DEBUG_SEVERITY_MEDIUM:
                        FBT_CORE_ERROR(message);
                        return;
                case GL_DEBUG_SEVERITY_LOW:
                        FBT_CORE_WARN(message);
                        return;
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                        FBT_CORE_TRACE(message);
                        return;
        }

        FBT_CORE_ASSERT(false, "Unknown severity level!");
}
*/

namespace Flibbert
{
	void OpenGLRendererBackend::InitGraphicsContext(RGFW_window* window)
	{
		RGFW_window_makeCurrent(window);
		int status = gladLoadGL((GLADloadfunc)RGFW_getProcAddress);
		assert(status);

		FBT_CORE_INFO("OpenGL Info:");
		FBT_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		FBT_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		FBT_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));
	}

	OpenGLRendererBackend::OpenGLRendererBackend()
	{
		// @todo would be great to use this, but macOS is stuck on OpenGL 4.1...
		// once metal is implemented, add this
		// #ifdef _DEBUG
		// 	glEnable(GL_DEBUG_OUTPUT);
		// 	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		// 	glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		//
		// 	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
		// GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE); #endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererBackend::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererBackend::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererBackend::Draw(const OpenGLVertexArray& va, const OpenGLIndexBuffer& ib,
	                                 const OpenGLShader& shader) const
	{
		va.Bind();
		ib.Bind();
		shader.Bind();

		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
} // namespace Flibbert
