#include "Flibbert/Core/Application.h"

#include "Flibbert/Core/ApplicationSubsystem.h"
#include "Flibbert/Core/ClassRegistry.h"
#include "Flibbert/Core/Platform.h"
#include "Flibbert/Renderer/Renderer.h"
#include "Platform/OS/Desktop/Window.h"

#include <imgui.h>

#include <filesystem>
#include <memory>
#include <vector>

namespace Flibbert {

Application* Application::s_Instance = nullptr;

Application::Application(const ApplicationInfo& info)
{
	ZoneScoped;

	if (!FBT_CORE_ENSURE_MSG(s_Instance == nullptr, "Application already exists!")) {
		return;
	}
	s_Instance = this;

	TracySetProgramName(info.Name.c_str());

	// Set the working directory to be the folder containing the exe by default,
	// eventually add an option to replace it
	std::filesystem::path executablePath;
	if (Platform::GetExecutablePath(executablePath)) {
		std::filesystem::current_path(executablePath.parent_path());
	}

	InitializeSubsystems();

	{
		ZoneNamedN(ZoneWindowInit, "Window Initialization", true);
		WindowProps props;
		props.Title = info.Name;
		m_Window = std::make_unique<Window>(props);
		(void)m_Window->OnWindowClosed.AddDynamic(this, Application::HandleWindowClosed);
	}

	{
		ZoneNamedN(ZoneRendererInit, "Renderer Initialization", true);
		m_Renderer = std::make_unique<Renderer>();
	}

	{
		ZoneNamedN(ZoneImguiInit, "ImGui Initialization", true);
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		m_Window->InitImGui();
		m_Renderer->InitImGui();
		ImGui::StyleColorsDark();
	}

	m_Running = true;
}

Application::~Application()
{
	ZoneScoped;

	m_Renderer->ShutdownImGui();
	m_Window->ShutdownImGui();
	ImGui::DestroyContext();

	ShutdownSubsystems();

	s_Instance = nullptr;
}

Application& Application::Get()
{
	return *s_Instance;
}

void Application::InitializeSubsystems()
{
	std::vector<const ClassRegistry::ClassInfo*> subsystemClasses;
	ClassRegistry::GetChildClasses<ApplicationSubsystem>(subsystemClasses);

	for (auto subsystemClass : subsystemClasses) {
		auto subsystem = ClassRegistry::Create<ApplicationSubsystem>(subsystemClass);
		m_Subsystems.push_back(subsystem);
		subsystem->Initialize(this);
	}
}

void Application::ShutdownSubsystems()
{
	for (auto& subsystem : m_Subsystems) {
		subsystem->Shutdown();
	}
	m_Subsystems.clear();
}

void Application::Run()
{
	ZoneScoped;

	while (m_Running) {
		m_Window->ProcessEvents();

		const double time = Platform::GetTime();
		m_FrameTime = time - m_LastFrameTime;
		m_LastFrameTime = time;

		{
			ZoneNamedN(OnUpdateFrame, "OnUpdate", true);

			OnUpdate(m_FrameTime);
		}

		{
			ZoneNamedN(OnUpdateFrame, "OnRender", true);

			m_Renderer->Clear();
			OnRender();
		}

		{
			ZoneNamedN(ImGuiFrame, "ImGuiFrame", true);
			m_Window->BeginImGuiFrame();
			m_Renderer->BeginImGuiFrame();
			{
				ZoneNamedN(ImGuiNewFrame, "ImGui::NewFrame()", true);
				ImGui::NewFrame();
			}

			OnImguiRender();

			{
				ZoneNamedN(ImGuiRender, "ImGui::Render()", true);
				ImGui::Render();
			}
			m_Renderer->EndImGuiFrame();
		}

#if FBT_PROFILING_ENABLED
		m_Renderer->CaptureTracyFrameImage();
#endif

		m_Window->SwapBuffers();
		FrameMark;
#if FBT_PROFILING_ENABLED
		m_Renderer->CollectTracyGPUTraces();
#endif
	}
}

void Application::Close()
{
	m_Running = false;
}

void Application::HandleWindowClosed(Window& window)
{
	Close();
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
