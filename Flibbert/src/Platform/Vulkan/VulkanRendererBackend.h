#pragma once

#include "Flibbert/Renderer/RendererBackend.h"

namespace Flibbert
{
	class Window;

	class VulkanRendererBackend : public RendererBackend
	{
	public:
		VulkanRendererBackend();
		~VulkanRendererBackend() override;

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
	};
} // namespace Flibbert
