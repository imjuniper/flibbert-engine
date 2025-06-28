#include <glad/glad.h>

#include <imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#define RGFW_IMGUI_IMPLEMENTATION
#include <rgfw/imgui_impl_rgfw.h>

#include <rgfw/RGFW.h>

#include "Flibbert/Core/Application.h"
#include "Platform/OpenGL/Renderer.h"

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
		RGFW_setGLHint(RGFW_glMajor, 4);
#ifdef FBT_PLATFORM_MACOS
		/* Set OpenGL version to 4.1 on macOS */
		RGFW_setGLHint(RGFW_glMinor, 1);
#else
		RGFW_setGLHint(RGFW_glMinor, 6);
#endif
		RGFW_setGLHint(RGFW_glProfile, RGFW_glCore);

		/* Create a windowed mode window and its src context */
		m_WindowHandle = RGFW_createWindow("OpenGL Learning", RGFW_RECT(0, 0, 960, 540),
		                                   RGFW_windowCenter);
		if (!m_WindowHandle) return;

		RGFW_window_makeCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)RGFW_getProcAddress);
		assert(status);

		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplRgfw_InitForOpenGL(m_WindowHandle, true);
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
		RGFW_window_close(m_WindowHandle);
	}

	void Application::Run()
	{
		Renderer renderer;

		while (RGFW_window_shouldClose(m_WindowHandle) == RGFW_FALSE) {
			while (RGFW_window_checkEvent(m_WindowHandle))
				;
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplRgfw_NewFrame();
			ImGui::NewFrame();

			this->Render(m_TimeStep);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			RGFW_window_swapBuffers(m_WindowHandle);

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
