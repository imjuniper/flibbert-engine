#include "fbtpch.h"
#include <glad/glad.h>

#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "Flibbert/Core/Application.h"
#include "Platform/OpenGL/Renderer.h"

static Flibbert::Application* s_Instance = nullptr;

namespace Flibbert {
	Application::Application() {
		s_Instance = this;
		Init();
	}

	Application::~Application() {
		Shutdown();
		s_Instance = nullptr;
	}

	Application& Application::Get() {
		return *s_Instance;
	}

	void Application::Init() {
		if (!glfwInit())
			return;

		/* Set OpenGL version to 4.1 on macOS */
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifdef FBT_PLATFORM_MACOS
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

		/* Create a windowed mode window and its src context */
		m_WindowHandle = glfwCreateWindow(960, 540, "OpenGL Learning", nullptr, nullptr);

		if (!m_WindowHandle)
			return;

		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		assert(status);

		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init("#version 410");
		ImGui_ImplGlfw_InitForOpenGL(m_WindowHandle, true);
		ImGui::StyleColorsDark();
	}

	void Application::Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}

	void Application::Run() {
		Renderer renderer;

		while (!glfwWindowShouldClose(m_WindowHandle)) {
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			this->Render(m_TimeStep);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(m_WindowHandle);
			glfwPollEvents();

			float time = GetTime();
			m_FrameTime = time - m_LastFrameTime;
			m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
			m_LastFrameTime = time;
		}
	}

	float Application::GetTime() {
		return (float)glfwGetTime();
	}
} // namespace Flibbert
