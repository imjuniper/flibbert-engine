#include "Platform/OpenGL/OpenGLRendererBackend.h"

#include "Flibbert/Core/Application.h"

#include "Platform/Desktop/Window.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <backends/imgui_impl_opengl3.h>

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
	OpenGLRendererBackend::OpenGLRendererBackend()
	{
		Window& window = Application::Get().GetWindow();
		RGFW_window_makeCurrent(window.GetNativeWindow());
		int status = gladLoadGL(RGFW_getProcAddress);
		FBT_CORE_ENSURE(status);

		window.OnWindowResized.Add(FBT_BIND_EVENT(OpenGLRendererBackend::OnWindowResized));

		FBT_CORE_INFO("OpenGL Info:");
		FBT_CORE_INFO("  Vendor: {0}",
		              reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		FBT_CORE_INFO("  Renderer: {0}",
		              reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		FBT_CORE_INFO("  Version: {0}",
		              reinterpret_cast<const char*>(glGetString(GL_VERSION)));

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

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererBackend::InitImGui()
	{
#ifdef FBT_PLATFORM_MACOS
		/* Set OpenGL version to 4.1 on macOS */
		ImGui_ImplOpenGL3_Init("#version 410");
#else
		ImGui_ImplOpenGL3_Init("#version 460");
#endif
	}

	void OpenGLRendererBackend::BeginImGuiFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
	}

	void OpenGLRendererBackend::EndImGuiFrame()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void OpenGLRendererBackend::ShutdownImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
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

	void OpenGLRendererBackend::Draw(const std::shared_ptr<VertexArray>& vertexArray,
	                                 const std::shared_ptr<Shader>& shader,
	                                 const glm::mat4 transform) const
	{
		vertexArray->Bind();
		shader->Bind();

		shader->SetUniformMat4f("u_Transform", transform);

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(),
		               GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererBackend::Present() const
	{
		// @todo figure out how to do that without rgfw?
		RGFW_window_swapBuffers(Application::Get().GetWindow().GetNativeWindow());
	}

	void OpenGLRendererBackend::OnWindowResized(Window& window, const glm::u32vec2& size)
	{
		glViewport(0, 0, size.x, size.y);
	}
} // namespace Flibbert
