#include "Platform/Vulkan/VulkanRendererBackend.h"

#include "Flibbert/Core/Application.h"

#include "Platform/Desktop/Window.h"

namespace Flibbert
{
	VulkanRendererBackend::VulkanRendererBackend()
	{
		ZoneScoped;

		Window& window = Application::Get().GetWindow();

		m_WindowResizedDelegate =
			window.OnWindowResized.AddDynamic(this, VulkanRendererBackend::OnWindowResized);

#if FBT_PROFILING_ENABLED
		SetupTracyFrameImageData();
#endif
	}

	VulkanRendererBackend::~VulkanRendererBackend()
	{
#if FBT_PROFILING_ENABLED
		CleanupTracyFrameImageData();
#endif
	}

	void VulkanRendererBackend::InitImGui()
	{
		ZoneScoped;
	}

	void VulkanRendererBackend::BeginImGuiFrame()
	{
		ZoneScoped;
	}

	void VulkanRendererBackend::EndImGuiFrame()
	{
		ZoneScoped;
	}

	void VulkanRendererBackend::ShutdownImGui()
	{
		ZoneScoped;
	}

	void VulkanRendererBackend::SetClearColor(const glm::vec4& color)
	{
	}

	void VulkanRendererBackend::Clear()
	{
	}

	void VulkanRendererBackend::Draw(const std::shared_ptr<VertexArray>& vertexArray,
	                                 const std::shared_ptr<Shader>& shader) const
	{
		ZoneScoped;
	}

#if FBT_PROFILING_ENABLED
	void VulkanRendererBackend::SetupTracyFrameImageData()
	{
	}

	void VulkanRendererBackend::CleanupTracyFrameImageData()
	{
	}

	void VulkanRendererBackend::CaptureTracyFrameImage()
	{
	}

	void VulkanRendererBackend::CollectTracyGPUTraces()
	{
	}
#endif

	void VulkanRendererBackend::OnWindowResized(Window& window, const glm::u32vec2& size)
	{
		ZoneScoped;
	}
} // namespace Flibbert
