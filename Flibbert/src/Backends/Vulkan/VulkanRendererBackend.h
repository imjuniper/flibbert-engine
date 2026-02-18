#pragma once

#include "Flibbert/Renderer/RendererBackend.h"

namespace Flibbert {

class Window;

class VulkanRendererBackend : public IRendererBackend
{
public:
	VulkanRendererBackend();
	~VulkanRendererBackend() override;

	void SetClearColor(const glm::vec4& color) override;
	void Clear() override;

	void Submit(const std::shared_ptr<IVertexArray>& vertexArray,
	          const std::shared_ptr<IShader>& shader) const override;

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
