#include "Backends/OpenGL/OpenGLRendererBackend.h"

#include "Backends/OpenGL/OpenGLProfiling.h"
#include "Flibbert/Core/Application.h"

#include "Backends/OpenGL/OpenGLBuffer.h"
#include "Backends/OpenGL/OpenGLShader.h"
#include "Backends/OpenGL/OpenGLVertexArray.h"
#include "Platform/Desktop/Window.h"
#include "SDL3/SDL_video.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad.h>

void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length,
                           const char* message, const void* userParam)
{
	const auto srcString = [source]() {
		switch (source) {
		case GL_DEBUG_SOURCE_API:
			return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION:
			return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER:
		default:
			return "OTHER";
		}
	}();

	const auto typeString = [type]() {
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY:
			return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE:
			return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER:
			return "MARKER";
		case GL_DEBUG_TYPE_OTHER:
		default:
			return "OTHER";
		}
	}();

	const auto formattedMessage = std::format("{0}, {1}, {2}: {3}", srcString, typeString, id, message);

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		FBT_CORE_CRITICAL(formattedMessage);
		return;
	case GL_DEBUG_SEVERITY_MEDIUM:
		FBT_CORE_ERROR(formattedMessage);
		return;
	case GL_DEBUG_SEVERITY_LOW:
		FBT_CORE_WARN(formattedMessage);
		return;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		FBT_CORE_TRACE(formattedMessage);
		return;
	}

	FBT_CORE_ENSURE_MSG(false, "Unknown severity level!");
}

namespace Flibbert {

OpenGLRendererBackend::OpenGLRendererBackend()
{
	FBT_PROFILE_FUNCTION();

	Window& window = Application::Get().GetWindow();

	{
		FBT_PROFILE_SCOPE("OpenGL Context Initialization");

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		m_GlContext = SDL_GL_CreateContext(window.GetNativeWindow());

		int status = gladLoadGL(SDL_GL_GetProcAddress);
		FBT_CORE_ENSURE(status);

		FBT_PROFILE_GPU_INIT();

		FBT_CORE_INFO("OpenGL Info:");
		FBT_CORE_INFO("\tVendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		FBT_CORE_INFO("\tRenderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		FBT_CORE_INFO("\tVersion: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	m_WindowResizedDelegate = window.OnWindowResized.AddDynamic(this, OpenGLRendererBackend::OnWindowResized);

#ifdef FBT_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if FBT_PROFILING_ENABLED
	SetupProfilerFrameImageData();
#endif
}

OpenGLRendererBackend::~OpenGLRendererBackend()
{
#if FBT_PROFILING_ENABLED
	CleanupProfilerFrameImageData();
#endif
}

void OpenGLRendererBackend::SetClearColor(const glm::vec4& color)
{
	FBT_PROFILE_FUNCTION();

	IRendererBackend::SetClearColor(color);
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererBackend::Clear()
{
	FBT_PROFILE_FUNCTION();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererBackend::Submit(const std::shared_ptr<IVertexArray>& vertexArray,
                                   const std::shared_ptr<IShader>& shader) const
{
	FBT_PROFILE_FUNCTION();
	FBT_PROFILE_GPU_SCOPE("OpenGLRendererBackend::Draw");

	vertexArray->Bind();
	shader->Bind();

	glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

#if FBT_PROFILING_ENABLED
void OpenGLRendererBackend::SetupProfilerFrameImageData()
{
	glGenTextures(4, m_ProfilerTexture);
	glGenFramebuffers(4, m_ProfilerFramebuffer);
	glGenBuffers(4, m_ProfilerPBO);
	for (int i = 0; i < 4; i++) {
		glBindTexture(GL_TEXTURE_2D, m_ProfilerTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 320, 180, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ProfilerFramebuffer[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ProfilerTexture[i], 0);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, m_ProfilerPBO[i]);
		glBufferData(GL_PIXEL_PACK_BUFFER, 320 * 180 * 4, nullptr, GL_STREAM_READ);
	}
}

void OpenGLRendererBackend::CleanupProfilerFrameImageData()
{
	glDeleteBuffers(4, m_ProfilerPBO);
	glDeleteFramebuffers(4, m_ProfilerFramebuffer);
	glDeleteTextures(4, m_ProfilerTexture);
}

void OpenGLRendererBackend::CaptureProfilerFrameImage()
{
	FBT_PROFILE_FUNCTION();

	assert(m_ProfilerQueue.empty() || m_ProfilerQueue.front() != m_ProfilerIdx);

	const auto windowSize = Application::Get().GetWindow().GetSize();
	constexpr auto captureSize = glm::uvec2(320, 180);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ProfilerFramebuffer[m_ProfilerIdx]);
	glBlitFramebuffer(0, 0, windowSize.x, windowSize.y, 0, 0, captureSize.x, captureSize.y, GL_COLOR_BUFFER_BIT,
	                  GL_LINEAR);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_ProfilerFramebuffer[m_ProfilerIdx]);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_ProfilerPBO[m_ProfilerIdx]);
	glReadPixels(0, 0, captureSize.x, captureSize.y, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	m_ProfilerFence[m_ProfilerIdx] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	m_ProfilerQueue.emplace_back(m_ProfilerIdx);
	m_ProfilerIdx = (m_ProfilerIdx + 1) % 4;

	// Quarter resolution captures
	while (!m_ProfilerQueue.empty()) {
		const auto i = m_ProfilerQueue.front();
		if (glClientWaitSync(m_ProfilerFence[i], 0, 0) == GL_TIMEOUT_EXPIRED)
			break;

		glDeleteSync(m_ProfilerFence[i]);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, m_ProfilerPBO[i]);
		auto capture =
		    glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, captureSize.x * captureSize.y * 4, GL_MAP_READ_BIT);
		FBT_PROFILE_GPU_IMAGE(capture, captureSize.x, captureSize.y, m_ProfilerQueue.size(), true);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		m_ProfilerQueue.erase(m_ProfilerQueue.begin());
	}
}

void OpenGLRendererBackend::CollectProfilerGPUTraces()
{
	FBT_PROFILE_GPU_COLLECT();
}
#endif

void OpenGLRendererBackend::OnWindowResized(Window& window, const glm::u32vec2& size)
{
	FBT_PROFILE_FUNCTION();

	glViewport(0, 0, size.x, size.y);
}

} // namespace Flibbert
