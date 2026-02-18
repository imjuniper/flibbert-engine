#pragma once

#include "Flibbert/Core/ClassRegistry.h"
#include "Flibbert/Renderer/RendererBackend.h"

#if FBT_PROFILING_ENABLED
	#include <glad.h>
#endif

struct SDL_GLContextState;

namespace Flibbert {

class Window;

class OpenGLRendererBackend : public IRendererBackend
{
	FBTCLASS(OpenGLRendererBackend, IRendererBackend)

public:
	OpenGLRendererBackend();
	~OpenGLRendererBackend() override;

	void SetClearColor(const glm::vec4& color) override;
	void Clear() override;

	void Submit(const std::shared_ptr<IVertexArray>& vertexArray,
	            const std::shared_ptr<IShader>& shader) const override;

#if FBT_PROFILING_ENABLED
	void SetupProfilerFrameImageData();
	void CleanupProfilerFrameImageData();
	void CaptureProfilerFrameImage() override;
	void CollectProfilerGPUTraces() override;
#endif

private:
	void OnWindowResized(Window& window, const glm::u32vec2& size);

private:
	DelegateHandle m_WindowResizedDelegate;

	SDL_GLContextState* m_GlContext;

#if FBT_PROFILING_ENABLED
	GLuint m_ProfilerTexture[4];
	GLuint m_ProfilerFramebuffer[4];
	GLuint m_ProfilerPBO[4];
	GLsync m_ProfilerFence[4];
	int m_ProfilerIdx = 0;
	std::vector<int> m_ProfilerQueue;
#endif
};

} // namespace Flibbert
