#include "Platform/Metal/MetalRendererBackend.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Core/Log.h"
#include "Platform/Desktop/Window.h"

#include <imgui_impl_metal_cpp.h>

#define RGFW_NATIVE
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
	// temp until i figure out the rest of the stuff
	static MTL::CommandBuffer* s_CommandBuffer;
	static MTL::RenderCommandEncoder* s_ImGuiEncoder;
	static MTL::ClearColor s_MtlClearColor;

	MetalRendererBackend::MetalRendererBackend()
	{
		Window& window = Application::Get().GetWindow();
		RGFW_window* windowHandle = window.GetNativeWindow();

		m_Device = MTL::CreateSystemDefaultDevice();
		m_CommandQueue = m_Device->newCommandQueue();

		m_Layer = CA::MetalLayer::layer();
		m_Layer->setDevice(m_Device);
		m_Layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

		auto scale = RGFW_window_getMonitor(windowHandle).pixelRatio;
		m_Layer->setDrawableSize(CGSizeMake(windowHandle->w * scale, windowHandle->h * scale));

		m_View = reinterpret_cast<NS::View*>(RGFW_window_getView_OSX(windowHandle));
		m_View->setLayer(m_Layer);
		m_View->setWantsLayer(true);

		m_WindowResizedDelegate =
		    window.OnWindowResized.AddDynamic(this, MetalRendererBackend::OnWindowResized);
	}

	MetalRendererBackend::~MetalRendererBackend()
	{
		m_Layer->release();
		m_CommandQueue->release();
		m_Device->release();
	}

	void MetalRendererBackend::BeginFrame()
	{
		ZoneScoped;

		m_Drawable = m_Layer->nextDrawable();

		auto* imGuiColorAttachment = m_ImGuiRenderPassDescriptor->colorAttachments()->object(0);
		imGuiColorAttachment->setClearColor(s_MtlClearColor); // should be on main pass
		imGuiColorAttachment->setLoadAction(MTL::LoadActionClear);
		imGuiColorAttachment->setStoreAction(MTL::StoreActionStore);
		imGuiColorAttachment->setTexture(m_Drawable->texture());

		s_CommandBuffer = m_CommandQueue->commandBuffer();

		static const auto uiEncoderLabel = NS::String::string("UIRenderEncoder", NS::UTF8StringEncoding);
		s_ImGuiEncoder = s_CommandBuffer->renderCommandEncoder(m_ImGuiRenderPassDescriptor);
		s_ImGuiEncoder->setLabel(uiEncoderLabel);
		s_ImGuiEncoder->pushDebugGroup(uiEncoderLabel);

		ImGui_ImplMetal_NewFrame(m_ImGuiRenderPassDescriptor);
	}

	void MetalRendererBackend::EndFrame()
	{
		ZoneScoped;

		ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), s_CommandBuffer, s_ImGuiEncoder);

		s_ImGuiEncoder->popDebugGroup();
		s_ImGuiEncoder->endEncoding();

		s_CommandBuffer->presentDrawable(m_Drawable);
		s_CommandBuffer->commit();

		m_Drawable->release(); // is this necessary?
	}

	void MetalRendererBackend::InitImGui()
	{
		ZoneScoped;

		m_ImGuiRenderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
		ImGui_ImplMetal_Init(m_Device);
	}

	void MetalRendererBackend::ShutdownImGui()
	{
		ZoneScoped;

		ImGui_ImplMetal_Shutdown();
		m_ImGuiRenderPassDescriptor->release();
	}

	void MetalRendererBackend::SetClearColor(const glm::vec4& color)
	{
		RendererBackend::SetClearColor(color);
		s_MtlClearColor = {color.r, color.g, color.b, color.a};
	}

	void MetalRendererBackend::Draw(const std::shared_ptr<VertexArray>& vertexArray,
	                                const std::shared_ptr<Shader>& shader) const
	{
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::Draw");
	}

	void MetalRendererBackend::OnWindowResized(Window& window, const glm::u32vec2& size)
	{
		ZoneScoped;

		auto scale = RGFW_window_getMonitor(window.GetNativeWindow()).pixelRatio;
		m_Layer->setDrawableSize(CGSizeMake(size.x * scale, size.y * scale));
	}
} // namespace Flibbert
