#include "Platform/Metal/MetalRendererBackend.h"

#include <rgfw/RGFW.h>

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace Flibbert
{
	void MetalRendererBackend::InitGraphicsContext(RGFW_window* window)
	{
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::InitGraphicsContext");
	}

	MetalRendererBackend::MetalRendererBackend(RGFW_window* window)
	{
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

	void MetalRendererBackend::SetClearColor(const glm::vec4& color)
	{
		RendererBackend::SetClearColor(color);
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::SetClearColor");
	}

	void MetalRendererBackend::Clear()
	{
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::Clear");
	}

	void MetalRendererBackend::Draw(const VertexArray& vertexArray,
	                                const IndexBuffer& indexBuffer, Shader& shader,
	                                const glm::mat4& viewProjection,
	                                const glm::mat4& transform) const
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
