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

	MetalRendererBackend::MetalRendererBackend()
	{
		RGFW_window* window = Application::Get().GetWindow().GetNativeWindow();

		m_Device = MTL::CreateSystemDefaultDevice();
		m_CommandQueue = m_Device->newCommandQueue();

		m_Layer = CA::MetalLayer::layer();
		m_Layer->setDevice(m_Device);
		m_Layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

		m_View = reinterpret_cast<NS::View*>(window->src.view);
		m_View->setLayer(m_Layer);
		m_View->setWantsLayer(true);
	}

	MetalRendererBackend::~MetalRendererBackend()
	{
		m_Layer->release();
		m_CommandQueue->release();
		m_Device->release();
	}

	void MetalRendererBackend::InitImGui()
	{
		ZoneScoped;

		ImGui_ImplMetal_Init(m_Device);
		m_ImGuiRenderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
	}

	void MetalRendererBackend::BeginImGuiFrame()
	{
		ZoneScoped;

		{
			// @todo this should be in another function, probably need to
			// have a RendererBackend::BeginFrame and EndFrame
			auto size = Application::Get().GetWindow().GetSize();
			m_Layer->setDrawableSize(CGSizeMake(size.x, size.y));
			m_Drawable = m_Layer->nextDrawable();

			auto* imGuiColorAttachment = m_ImGuiRenderPassDescriptor->colorAttachments()->object(0);
			// imGuiColorAttachment->setClearColor(MTL::ClearColor(1.0, 0.0, 0.0, 1.0));
			imGuiColorAttachment->setLoadAction(MTL::LoadActionClear);
			imGuiColorAttachment->setStoreAction(MTL::StoreActionStore);
			imGuiColorAttachment->setTexture(m_Drawable->texture());

			s_CommandBuffer = m_CommandQueue->commandBuffer();
		}

		static const auto uiEncoderLabel = NS::String::string("UIRenderEncoder", NS::UTF8StringEncoding);
		s_ImGuiEncoder = s_CommandBuffer->renderCommandEncoder(m_ImGuiRenderPassDescriptor);
		s_ImGuiEncoder->setLabel(uiEncoderLabel);
		s_ImGuiEncoder->pushDebugGroup(uiEncoderLabel);

		ImGui_ImplMetal_NewFrame(m_ImGuiRenderPassDescriptor);
	}

	void MetalRendererBackend::EndImGuiFrame()
	{
		ZoneScoped;

		ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), s_CommandBuffer, s_ImGuiEncoder);

		s_ImGuiEncoder->popDebugGroup();
		s_ImGuiEncoder->endEncoding();

		{
			// @todo this should be in another function, probably need to
			// have a RendererBackend::BeginFrame and EndFrame
			s_CommandBuffer->presentDrawable(m_Drawable);
			s_CommandBuffer->commit();
		}
	}

	void MetalRendererBackend::ShutdownImGui()
	{
		ZoneScoped;

		m_ImGuiRenderPassDescriptor->release();
		ImGui_ImplMetal_Shutdown();
	}

	void MetalRendererBackend::SetClearColor(const glm::vec4& color)
	{
		RendererBackend::SetClearColor(color);
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::SetClearColor");
	}

	void MetalRendererBackend::Clear()
	{
		static bool bAlreadyLogged = false;
		if (!bAlreadyLogged) {
			bAlreadyLogged = true;
			FBT_CORE_ERROR("Unimplemented MetalRendererBackend::Clear");
		}
	}

	void MetalRendererBackend::Draw(const std::shared_ptr<VertexArray>& vertexArray,
	                                const std::shared_ptr<Shader>& shader) const
	{
		FBT_CORE_ERROR("Unimplemented MetalRendererBackend::Draw");
	}
} // namespace Flibbert
