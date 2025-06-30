#include "Flibbert/Core/Application.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Flibbert/Renderer/RendererBackend.h"
#include "Platform/Desktop/Window.h"

#include <external/imgui/backends/imgui_impl_opengl3.h>
#include <external/imgui/imgui.h>
#define RGFW_IMGUI_IMPLEMENTATION

#include <external/imgui_impl_rgfw.h>

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
		m_Renderer = new Renderer();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplRgfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
#ifdef FBT_PLATFORM_MACOS
		/* Set OpenGL version to 4.1 on macOS */
		ImGui_ImplOpenGL3_Init("#version 410");
#else
		ImGui_ImplOpenGL3_Init("#version 460");
#endif
		ImGui::StyleColorsDark();
	}

	Application::~Application()
	{
		FBT_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
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
			m_Renderer->GetBackend()->Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplRgfw_NewFrame();
			ImGui::NewFrame();

			this->Render(m_TimeStep);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			RGFW_window_swapBuffers(m_Window->GetNativeWindow());

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
