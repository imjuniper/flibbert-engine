#include "Application.h"

#include "Platform/OpenGL/Renderer.h"
#include "Platform/Window.h"

#include <external/imgui/backends/imgui_impl_opengl3.h>
#include <external/imgui/imgui.h>
#define RGFW_IMGUI_IMPLEMENTATION
#include <external/imgui_impl_rgfw.h>

static Flibbert::Application* s_Instance = nullptr;

namespace Flibbert
{
	Application::Application()
	{
		s_Instance = this;
		Init();
	}

	Application::~Application()
	{
		Shutdown();
		s_Instance = nullptr;
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}

	void Application::Init()
	{
		m_Window = new Window();

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

	void Application::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplRgfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::Run()
	{
		Renderer renderer;

		while (RGFW_window_shouldClose(m_Window->GetNativeWindow()) == RGFW_FALSE) {
			while (RGFW_window_checkEvent(m_Window->GetNativeWindow()))
				;
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplRgfw_NewFrame();
			ImGui::NewFrame();

			this->Render(m_TimeStep);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			RGFW_window_swapBuffers(m_Window->GetNativeWindow());

			float time = GetTime();
			m_FrameTime = time - m_LastFrameTime;
			m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
			m_LastFrameTime = time;
		}
	}

	float Application::GetTime()
	{
		return (float)RGFW_getTime();
	}
} // namespace Flibbert
