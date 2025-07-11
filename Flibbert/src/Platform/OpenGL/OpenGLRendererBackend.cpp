#include "Platform/OpenGL/OpenGLRendererBackend.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <rgfw/RGFW.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad.h>

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

	FBT_CORE_ENSURE_MSG(false, "Unknown severity level!");
}

namespace Flibbert
{
	void OpenGLRendererBackend::InitGraphicsContext(RGFW_window* window)
	{
		RGFW_window_makeCurrent(window);
		int status = gladLoadGL(RGFW_getProcAddress);
		FBT_CORE_ENSURE(status);

		FBT_CORE_INFO("OpenGL Info:");
		FBT_CORE_INFO("  Vendor: {0}",
		              reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		FBT_CORE_INFO("  Renderer: {0}",
		              reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		FBT_CORE_INFO("  Version: {0}",
		              reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	OpenGLRendererBackend::OpenGLRendererBackend()
	{
		// When removing support for OpenGL in macOS, migrate to 4.3+ and remove extension
		// usage instead maybe?
#ifndef NDEBUG
		if (GLAD_GL_KHR_debug) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
			                      GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		}
#endif

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererBackend::SetClearColor(const glm::vec4& color)
	{
		RendererBackend::SetClearColor(color);
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererBackend::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererBackend::Draw(const VertexArray& vertexArray,
	                                 const IndexBuffer& indexBuffer, Shader& shader,
	                                 const glm::mat4& transform) const
	{
		vertexArray.Bind();
		indexBuffer.Bind();
		shader.Bind();

		shader.SetUniformMat4f("u_Transform", transform);

		glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
} // namespace Flibbert
