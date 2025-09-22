#include "Platform/Metal/MetalRendererBackend.h"

#include <rgfw/RGFW.h>

#include "Flibbert/Core/Application.h"
#include "Platform/Desktop/Window.h"

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace Flibbert
{
	MetalRendererBackend::MetalRendererBackend()
	{
		RGFW_window* window = Application::Get().GetWindow().GetNativeWindow();

		m_Device = MTLCreateSystemDefaultDevice();
		m_CommandQueue = m_Device->newCommandQueue();
		m_Layer = CA::MetalLayer::layer();
		m_Layer->setDevice(m_Device);
		m_View = reinterpret_cast<NS::View*>(window->src.view);
		// @todo this crashes
		// m_View->setLayer(m_Layer);
		// m_View->setWantsLayer(true);
		// m_View->setOpaque(true);
	}

	MetalRendererBackend::~MetalRendererBackend() {
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::~MetalRendererBackend");
	}

	void MetalRendererBackend::InitImGui() {
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::InitImGui");
	}

	void MetalRendererBackend::BeginImGuiFrame() {
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::BeginImGuiFrame");
	}

	void MetalRendererBackend::EndImGuiFrame() {
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::EndImGuiFrame");
	}

	void MetalRendererBackend::ShutdownImGui() {
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::ShutdownImGui");
	}

	void MetalRendererBackend::SetClearColor(const glm::vec4& color)
	{
		RendererBackend::SetClearColor(color);
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::SetClearColor");
	}

	void MetalRendererBackend::Clear()
	{
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::Clear");
	}

	void MetalRendererBackend::Draw(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader) const
	{
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::Draw");
	}

	MTL::Device* MetalRendererBackend::GetDevice() const
	{
		return m_Device;
	}

	MTL::CommandQueue* MetalRendererBackend::GetCommandQueue() const
	{
		return m_CommandQueue;
	}
} // namespace Flibbert
