#pragma once

#include "Flibbert/Renderer/RendererBackend.h"

#if FBT_PROFILING_ENABLED
	#include <glad.h>
#endif

namespace Flibbert
{
	class Window;

	class OpenGLRendererBackend : public RendererBackend
	{
	public:
		OpenGLRendererBackend();
		~OpenGLRendererBackend() override;

		void BeginFrame() override;
		void EndFrame() override;

		void InitImGui() override;
		void ShutdownImGui() override;

		void SetClearColor(const glm::vec4& color) override;

		void Draw(const std::shared_ptr<VertexArray>& vertexArray,
			  const std::shared_ptr<Shader>& shader) const override;

#if FBT_PROFILING_ENABLED
		void SetupTracyFrameImageData();
		void CleanupTracyFrameImageData();
		void CaptureTracyFrameImage() override;
		void CollectTracyGPUTraces() override;
#endif

	private:
		void OnWindowResized(Window& window, const glm::u32vec2& size);

	private:
		DelegateHandle m_WindowResizedDelegate;

#if FBT_PROFILING_ENABLED
		GLuint m_TracyTexture[4];
		GLuint m_TracyFramebuffer[4];
		GLuint m_TracyPBO[4];
		GLsync m_TracyFence[4];
		int m_TracyIdx = 0;
		std::vector<int> m_TracyQueue ;
#endif
	};
} // namespace Flibbert
