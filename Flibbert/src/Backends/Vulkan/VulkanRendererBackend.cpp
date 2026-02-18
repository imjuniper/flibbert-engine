#include "Backends/Vulkan/VulkanRendererBackend.h"

#include "Flibbert/Core/Application.h"

#include "Platform/Desktop/Window.h"

namespace Flibbert {

VulkanRendererBackend::VulkanRendererBackend()
{
	FBT_PROFILE_FUNCTION();

	Window& window = Application::Get().GetWindow();

	m_WindowResizedDelegate = window.OnWindowResized.AddDynamic(this, VulkanRendererBackend::OnWindowResized);

#if FBT_PROFILING_ENABLED
	SetupProfilerFrameImageData();
#endif
}

VulkanRendererBackend::~VulkanRendererBackend()
{
#if FBT_PROFILING_ENABLED
	CleanupProfilerFrameImageData();
#endif
}

void VulkanRendererBackend::SetClearColor(const glm::vec4& color) {}

void VulkanRendererBackend::Clear() {}

void VulkanRendererBackend::Submit(const std::shared_ptr<IVertexArray>& vertexArray,
                                 const std::shared_ptr<IShader>& shader) const
{
	FBT_PROFILE_FUNCTION();
}

#if FBT_PROFILING_ENABLED
void VulkanRendererBackend::SetupProfilerFrameImageData() {}

void VulkanRendererBackend::CleanupProfilerFrameImageData() {}

void VulkanRendererBackend::CaptureProfilerFrameImage() {}

void VulkanRendererBackend::CollectProfilerGPUTraces() {}
#endif

void VulkanRendererBackend::OnWindowResized(Window& window, const glm::u32vec2& size)
{
	FBT_PROFILE_FUNCTION();
}

} // namespace Flibbert
