#include "Flibbert/Core/Application.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Flibbert/Renderer/RendererBackend.h"
#include "Platform/Desktop/Window.h"

#include "Platform/Metal/MetalRendererBackend.h"

#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_impl_metal_cpp.h>
#define RGFW_IMGUI_IMPLEMENTATION

#include <rgfw/imgui_impl_rgfw.h>

#include <Metal/Metal.hpp>

#define NS_STRING_FROM_CSTRING(STR) NS::String::string(STR, NS::UTF8StringEncoding)

namespace Flibbert
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		FBT_PROFILE_FUNCTION();

		// FBT_CORE_ASSERT(!s_Instance, "Application already exists!");
		if (s_Instance != nullptr) return;
		s_Instance = this;

		m_Window = new Window();
		m_Renderer = new Renderer(m_Window);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		MTL::Device* mtlDevice = nullptr;
		switch (Renderer::GetAPI()) {
			case Renderer::API::Metal:
				ImGui_ImplRgfw_InitForOther(m_Window->GetNativeWindow(), true);
				mtlDevice =
				    ((MetalRendererBackend*)m_Renderer->GetBackend())->GetDevice();
				ImGui_ImplMetal_Init(mtlDevice);
				m_ImGuiRenderPassDescriptor =
				    MTL::RenderPassDescriptor::alloc()->init();
				break;
			case Renderer::API::OpenGL:
				ImGui_ImplRgfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
#ifdef FBT_PLATFORM_MACOS
				/* Set OpenGL version to 4.1 on macOS */
				ImGui_ImplOpenGL3_Init("#version 410");
#else
				ImGui_ImplOpenGL3_Init("#version 460");
#endif
				break;
			default:
				break;
		}
		ImGui::StyleColorsDark();
	}

	Application::~Application()
	{
		FBT_PROFILE_FUNCTION();

		switch (Renderer::GetAPI()) {
			case Renderer::API::Metal:
				m_ImGuiRenderPassDescriptor->release();
				ImGui_ImplMetal_Shutdown();
				break;
			case Renderer::API::OpenGL:
				ImGui_ImplOpenGL3_Shutdown();
				break;
			default:
				break;
		}
		ImGui_ImplRgfw_Shutdown();
		ImGui::DestroyContext();

		s_Instance = nullptr;
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}

	void Application::Run()
	{
		while (RGFW_window_shouldClose(m_Window->GetNativeWindow()) == RGFW_FALSE) {
			while (RGFW_window_checkEvent(m_Window->GetNativeWindow()))
				;
			// m_Renderer->GetBackend()->Clear();

			auto mtlBackend = (MetalRendererBackend*)m_Renderer->GetBackend();
			switch (Renderer::GetAPI()) {
				case Renderer::API::Metal:
					ImGui_ImplMetal_NewFrame(m_ImGuiRenderPassDescriptor);
					break;
				case Renderer::API::OpenGL:
					ImGui_ImplOpenGL3_NewFrame();
					break;
				default:
					break;
			}
			ImGui_ImplRgfw_NewFrame();
			ImGui::NewFrame();

			this->Render(m_TimeStep);

			ImGui::Render();

			auto commandBuffer = mtlBackend->GetCommandQueue()->commandBuffer();
			MTL::RenderCommandEncoder* encoder =
			    commandBuffer->renderCommandEncoder(m_ImGuiRenderPassDescriptor);
			encoder->setLabel(NS_STRING_FROM_CSTRING("UIRenderEncoder"));
			encoder->pushDebugGroup(NS_STRING_FROM_CSTRING("UIRenderEncoder"));
			switch (Renderer::GetAPI()) {
				case Renderer::API::Metal:
					ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(),
					                               commandBuffer, encoder);
					break;
				case Renderer::API::OpenGL:
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
					break;
				default:
					break;
			}

			encoder->popDebugGroup();
			encoder->endEncoding();

			// RGFW_window_swapBuffers(m_Window->GetNativeWindow());

			const float time = GetTime();
			m_FrameTime = time - m_LastFrameTime;
			m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
			m_LastFrameTime = time;
		}
	}

	float Application::GetTime() const
	{
		return static_cast<float>(RGFW_getTime());
	}
} // namespace Flibbert
