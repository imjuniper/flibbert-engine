#include "Flibbert/Core/Application.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Flibbert/Renderer/RendererBackend.h"
#include "Platform/Desktop/Window.h"

#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#define RGFW_IMGUI_IMPLEMENTATION
#include <rgfw/imgui_impl_rgfw.h>

namespace Flibbert
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationInfo& info)
	{
		FBT_PROFILE_FUNCTION();

		if (!FBT_CORE_ENSURE_MSG(s_Instance == nullptr, "Application already exists!")) {
			return;
		}
		s_Instance = this;

		{
			FBT_PROFILE_SCOPE("Window Initialization");
			WindowProps props;
			props.Title = info.Name;
			m_Window = std::make_unique<Window>(props);
		}

		{
			FBT_PROFILE_SCOPE("Renderer Initialization");
			m_Renderer = std::make_unique<Renderer>();
		}

		{
			FBT_PROFILE_SCOPE("ImGui Initialization");
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
		// Should try to make the windowing loop thing more generic
		// Maybe the window should run the loop somehow?
		while (RGFW_window_shouldClose(m_Window->GetNativeWindow()) == RGFW_FALSE) {
			while (RGFW_window_checkEvent(m_Window->GetNativeWindow())) {
				RGFW_event& event = m_Window->GetNativeWindow()->event;
				switch (event.type) {
					case RGFW_quit:
						// Window closed
						break;

					case RGFW_windowMoved:
						m_Window->OnWindowMoved();
						break;

					// @todo implement smooth resize
					// https://github.com/ColleagueRiley/RGFW/blob/main/examples/smooth-resize/smooth-resize.c?rgh-link-date=2025-06-23T16%3A00%3A55.000Z
					case RGFW_windowResized:
						m_Window->OnWindowResized();
						break;

					case RGFW_keyPressed:
						break;
					case RGFW_keyReleased:
						break;

					case RGFW_mouseButtonPressed:
						break;
					case RGFW_mouseButtonReleased:
						break;
					case RGFW_mousePosChanged:
						OnMouseMove.Broadcast(
						    {event.vector.x, event.vector.y});
						break;

					default:
						break;
				}
			}

			m_Renderer->Clear();

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

	Window& Application::GetWindow() const
	{
		FBT_CORE_ENSURE(m_Window != nullptr);
		return *m_Window;
	}

	Renderer& Application::GetRenderer() const
	{
		FBT_CORE_ENSURE(m_Renderer != nullptr);
		return *m_Renderer;
	}
} // namespace Flibbert
