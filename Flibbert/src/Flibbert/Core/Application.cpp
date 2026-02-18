#include "Flibbert/Core/Application.h"

#include "Backends/OpenGL/OpenGLRendererBackend.h"
#include "Flibbert/Core/ApplicationSubsystem.h"
#include "Flibbert/Core/ClassRegistry.h"
#include "Flibbert/Core/Platform.h"
#include "Flibbert/Renderer/Renderer.h"
#include "Flibbert/Renderer/RendererBackend.h"
#include "Modules/InitializeModules.h"
#include "Platform/Desktop/Window.h"

#include <filesystem>
#include <memory>
#include <vector>

namespace Flibbert {

static void RegisterTypes()
{
	ClassRegistry::RegisterAbstractClass<IRendererBackend>();
	// @todo make this dynamic like modules
	ClassRegistry::RegisterAbstractClass<OpenGLRendererBackend>();
}

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

	RegisterTypes();
	Flibbert::Modules::InitializeModules();

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

	InitializeSubsystems();

	m_Running = true;
}

Application::~Application()
{
	ZoneScoped;

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

	m_LastFrameTime = Platform::GetTime() - (1.0 / 60);

	while (m_Running) {
		m_Window->ProcessEvents();

		const double time = Platform::GetTime();
		m_FrameTime = time - m_LastFrameTime;
		m_LastFrameTime = time;

		{
			ZoneNamedN(OnUpdateFrame, "OnUpdate", true);

			OnUpdate(m_FrameTime);
			for (auto subsystem : m_Subsystems) {
				subsystem->OnUpdate(m_FrameTime);
			}
		}

		m_Renderer->Clear();

		{
			ZoneNamedN(OnRenderFrame, "OnRender", true);

			OnRender();
			for (auto subsystem : m_Subsystems) {
				subsystem->OnRender();
			}
		}

		{
			ZoneNamedN(OnPostRenderFrame, "OnPostRender", true);

			for (auto subsystem : m_Subsystems) {
				subsystem->OnPostRender();
			}
		}

#if FBT_PROFILING_ENABLED
		m_Renderer->CaptureTracyFrameImage();
#endif

		m_Window->Present();
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
