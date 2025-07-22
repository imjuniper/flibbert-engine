#include "Flibbert/Core/Application.h"

#include "Flibbert/Core/Platform.h"
#include "Flibbert/Input/Input.h"
#include "Flibbert/Renderer/Renderer.h"
#include "Platform/Desktop/Window.h"

#include <imgui.h>

#include <filesystem>

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

		// Set the working directory to be the folder containing the exe by default,
		// eventually add an option to replace it
		std::filesystem::path executablePath;
		if (Platform::GetExecutablePath(executablePath)) {
			std::filesystem::current_path(executablePath.parent_path());
		}

		Input::Get().InputEventDispatch.Bind(
		    FBT_BIND_EVENT(Application::DispatchInputEvent));

		{
			FBT_PROFILE_SCOPE("Window Initialization");
			WindowProps props;
			props.Title = info.Name;
			m_Window = std::make_unique<Window>(props);
			(void)m_Window->OnWindowClosed.Add(
			    FBT_BIND_EVENT(Application::HandleWindowClosed));
		}

		{
			FBT_PROFILE_SCOPE("Renderer Initialization");
			m_Renderer = std::make_unique<Renderer>();
		}

		{
			FBT_PROFILE_SCOPE("ImGui Initialization");
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
			m_Window->InitImGui();
			m_Renderer->InitImGui();
			ImGui::StyleColorsDark();
		}

		m_Running = true;
	}

	Application::~Application()
	{
		FBT_PROFILE_FUNCTION();

		m_Renderer->ShutdownImGui();
		m_Window->ShutdownImGui();
		ImGui::DestroyContext();

		s_Instance = nullptr;
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}

	void Application::Run()
	{
		while (m_Running) {
			m_Window->ProcessEvents();

			const double time = Platform::GetTime();
			m_FrameTime = time - m_LastFrameTime;
			m_TimeStep = std::min(m_FrameTime, 0.0333);
			m_LastFrameTime = time;

			{
				FBT_PROFILE_SCOPE("Application::Run OnUpdate Frame");
				m_Renderer->Clear();
				// @todo see if using double would be better
				OnUpdate(static_cast<float>(m_FrameTime));
			}

			{
				FBT_PROFILE_SCOPE("Application::Run ImGui Frame");
				m_Window->BeginImGuiFrame();
				m_Renderer->BeginImGuiFrame();
				ImGui::NewFrame();

				OnImguiRender();

				ImGui::Render();
				m_Renderer->EndImGuiFrame();
			}

			m_Window->SwapBuffers();
		}
	}

	void Application::HandleWindowClosed(Window& window)
	{
		m_Running = false;
	}

	void Application::DispatchInputEvent(const std::shared_ptr<InputEvent>& event)
	{
		OnInput(event);
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
