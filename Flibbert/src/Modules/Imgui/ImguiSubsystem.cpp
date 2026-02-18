#include "ImguiSubsystem.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Debug/Profiling.h"

// @todo don't depend on SDL
#include "SDL3/SDL_video.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl3.h"
#include "imgui.h"

namespace Flibbert::Modules::Imgui {

void ImguiSubsystem::Initialize(Application* application)
{
	Super::Initialize(application);

	ZoneScoped;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForOpenGL(application->GetWindow().GetNativeWindow(), SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init("#version 460");

	(void)application->GetWindow().OnPreprocessEvent.AddDynamic(this, ThisClass::PreprocessWindowEvent);
}

void ImguiSubsystem::OnUpdate(double _)
{
	ZoneScoped;

	ImGui_ImplSDL3_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	{
		ZoneNamedN(ImGuiNewFrame, "ImGui::NewFrame()", true);
		ImGui::NewFrame();
	}
}

void ImguiSubsystem::OnPostRender()
{
	ZoneScoped;

	OnImguiRender.Broadcast();

	{
		ZoneNamedN(ImGuiRender, "ImGui::Render()", true);
		ImGui::Render();
	}

	ImDrawData* DrawData = ImGui::GetDrawData();
	ImGui_ImplOpenGL3_RenderDrawData(DrawData);
}

void ImguiSubsystem::Shutdown()
{
	ZoneScoped;

	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void ImguiSubsystem::PreprocessWindowEvent(SDL_Event& event)
{
	ImGui_ImplSDL3_ProcessEvent(&event);
}

} // namespace Flibbert::Modules::Imgui
