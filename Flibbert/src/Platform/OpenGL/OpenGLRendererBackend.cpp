#include "Platform/OpenGL/OpenGLRendererBackend.h"

#include "Flibbert/Core/Application.h"

#include "Platform/Desktop/Window.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <backends/imgui_impl_opengl3.h>

#define RGFW_OPENGL
#include <rgfw/RGFW.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad.h>

#include <tracy/TracyOpenGL.hpp>

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
		ZoneScoped;

		Window& window = Application::Get().GetWindow();

		RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
		hints->major = 4;
		hints->minor = 1;

		RGFW_window_createContext_OpenGL(window.GetNativeWindow(), hints);
		int status = gladLoadGL(RGFW_getProcAddress_OpenGL);
		FBT_CORE_ENSURE(status);

		TracyGpuContext;

		m_WindowResizedDelegate = window.OnWindowResized.AddDynamic(this, OpenGLRendererBackend::OnWindowResized);

		FBT_CORE_INFO("OpenGL Info:");
		FBT_CORE_INFO("  Vendor: {0}",
		              reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		FBT_CORE_INFO("  Renderer: {0}",
		              reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		FBT_CORE_INFO("  Version: {0}",
		              reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		// When removing support for OpenGL in macOS, migrate to 4.3+ and remove extension
		// usage instead maybe?
#ifdef FBT_DEBUG
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

#if FBT_PROFILING_ENABLED
		glGenTextures(4, m_TracyTexture);
		glGenFramebuffers(4, m_TracyFramebuffer);
		glGenBuffers(4, m_TracyPBO);
		for (int i = 0; i < 4; i++) {
			glBindTexture(GL_TEXTURE_2D, m_TracyTexture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 320, 180, 0, GL_RGBA,
			             GL_UNSIGNED_BYTE, nullptr);
			glBindFramebuffer(GL_FRAMEBUFFER, m_TracyFramebuffer[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			                       m_TracyTexture[i], 0);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, m_TracyPBO[i]);
			glBufferData(GL_PIXEL_PACK_BUFFER, 320 * 180 * 4, nullptr, GL_STREAM_READ);
		}
#endif
	}

	void OpenGLRendererBackend::InitImGui()
	{
		ZoneScoped;

#ifdef FBT_PLATFORM_MACOS
		/* Set OpenGL version to 4.1 on macOS */
		ImGui_ImplOpenGL3_Init("#version 410");
#else
		ImGui_ImplOpenGL3_Init("#version 460");
#endif
	}

	void OpenGLRendererBackend::BeginImGuiFrame()
	{
		ZoneScoped;

		ImGui_ImplOpenGL3_NewFrame();
	}

	void OpenGLRendererBackend::EndImGuiFrame()
	{
		ZoneScoped;

		ImDrawData* DrawData = ImGui::GetDrawData();
		ImGui_ImplOpenGL3_RenderDrawData(DrawData);
	}

	void OpenGLRendererBackend::ShutdownImGui()
	{
		ZoneScoped;

		ImGui_ImplOpenGL3_Shutdown();
	}

	void OpenGLRendererBackend::SetClearColor(const glm::vec4& color)
	{
		ZoneScoped;
		TracyGpuZone("OpenGLRendererBackend::SetClearColor");

		RendererBackend::SetClearColor(color);
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererBackend::Clear()
	{
		ZoneScoped;
		TracyGpuZone("OpenGLRendererBackend::Clear");

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererBackend::Draw(const std::shared_ptr<VertexArray>& vertexArray,
	                                 const std::shared_ptr<Shader>& shader) const
	{
		ZoneScoped;
		TracyGpuZone("OpenGLRendererBackend::Draw");

		vertexArray->Bind();
		shader->Bind();

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(),
		               GL_UNSIGNED_INT, nullptr);
	}

#if FBT_PROFILING_ENABLED
	void OpenGLRendererBackend::CaptureTracyFrameImage()
	{
		ZoneScoped;

		assert(m_TracyQueue.empty() || m_TracyQueue.front() != m_TracyIdx);

		const auto windowSize = Application::Get().GetWindow().GetSize();
		constexpr auto captureSize = glm::uvec2(320, 180);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_TracyFramebuffer[m_TracyIdx]);
		glBlitFramebuffer(0, 0, windowSize.x, windowSize.y, 0, 0, captureSize.x, captureSize.y,
		                  GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_TracyFramebuffer[m_TracyIdx]);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, m_TracyPBO[m_TracyIdx]);
		glReadPixels(0, 0, captureSize.x, captureSize.y, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		m_TracyFence[m_TracyIdx] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		m_TracyQueue.emplace_back(m_TracyIdx);
		m_TracyIdx = (m_TracyIdx + 1) % 4;

		// Quarter resolution captures
		while (!m_TracyQueue.empty()) {
			const auto i = m_TracyQueue.front();
			if (glClientWaitSync(m_TracyFence[i], 0, 0) == GL_TIMEOUT_EXPIRED) break;

			glDeleteSync(m_TracyFence[i]);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, m_TracyPBO[i]);
			auto capture = glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0,
			                            captureSize.x * captureSize.y * 4,
			                            GL_MAP_READ_BIT);
			FrameImage(capture, captureSize.x, captureSize.y, m_TracyQueue.size(), true);
			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
			m_TracyQueue.erase(m_TracyQueue.begin());
		}
	}

	void OpenGLRendererBackend::CollectTracyGPUTraces()
	{
		TracyGpuCollect;
	}
#endif

	void OpenGLRendererBackend::OnWindowResized(Window& window, const glm::u32vec2& size)
	{
		ZoneScoped;
		TracyGpuZone("OpenGLRendererBackend::OnWindowResized");

		glViewport(0, 0, size.x, size.y);
	}
} // namespace Flibbert
