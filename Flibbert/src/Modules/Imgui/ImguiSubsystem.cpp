#include "ImguiSubsystem.h"

#include "Flibbert/Core/Application.h"

#include "SDL3/SDL_events.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

static ImGuiKey SDL_ScancodeToImGuiKey(SDL_Scancode scancode)
{
	switch (scancode) {
	case SDL_SCANCODE_A:
		return ImGuiKey_A;
	case SDL_SCANCODE_Z:
		return ImGuiKey_Z;
	case SDL_SCANCODE_RETURN:
		return ImGuiKey_Enter;
	case SDL_SCANCODE_ESCAPE:
		return ImGuiKey_Escape;
	case SDL_SCANCODE_BACKSPACE:
		return ImGuiKey_Backspace;
	case SDL_SCANCODE_DELETE:
		return ImGuiKey_Delete;
	case SDL_SCANCODE_TAB:
		return ImGuiKey_Tab;
	case SDL_SCANCODE_LEFT:
		return ImGuiKey_LeftArrow;
	case SDL_SCANCODE_RIGHT:
		return ImGuiKey_RightArrow;
	case SDL_SCANCODE_UP:
		return ImGuiKey_UpArrow;
	case SDL_SCANCODE_DOWN:
		return ImGuiKey_DownArrow;
	case SDL_SCANCODE_HOME:
		return ImGuiKey_Home;
	case SDL_SCANCODE_END:
		return ImGuiKey_End;
	case SDL_SCANCODE_LCTRL:
		return ImGuiKey_LeftCtrl;
	case SDL_SCANCODE_RCTRL:
		return ImGuiKey_RightCtrl;
	case SDL_SCANCODE_LSHIFT:
		return ImGuiKey_LeftShift;
	case SDL_SCANCODE_RSHIFT:
		return ImGuiKey_RightShift;
	case SDL_SCANCODE_LALT:
		return ImGuiKey_LeftAlt;
	case SDL_SCANCODE_RALT:
		return ImGuiKey_RightAlt;
	// Extend this table as needed
	default:
		return ImGuiKey_None;
	}
}

namespace Flibbert::Modules::Imgui {

struct ImguiPlatformBackendData
{
	Window* Window;
	uint64_t LastTime;
};

static ImguiPlatformBackendData* GetPlatformBackendData() {
	return static_cast<ImguiPlatformBackendData*>(ImGui::GetIO().BackendPlatformUserData);
}

void ImguiSubsystem::Initialize(Application* application)
{
	Super::Initialize(application);

	FBT_PROFILE_FUNCTION();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();

	ImguiPlatformBackendData* bd = new ImguiPlatformBackendData();
	bd->Window = &application->GetWindow();

	io.BackendPlatformUserData = (void*)bd;
	io.BackendPlatformName = "imgui_impl_flibbert";
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

	ImGui_ImplOpenGL3_Init("#version 460");

	(void)application->GetWindow().OnPreprocessEvent.AddDynamic(this, ThisClass::PreprocessWindowEvent);
}

void ImguiSubsystem::OnUpdate(double ts)
{
	FBT_PROFILE_FUNCTION();

	ImguiPlatformBackendData* bd = GetPlatformBackendData();
	ImGuiIO& io = ImGui::GetIO();

	// Display size
	const auto size = GetApplication()->GetWindow().GetSize();
	io.DisplaySize = ImVec2((float)size.x, (float)size.y);

	// // For HiDPI: set DisplayFramebufferScale if drawable size differs
	// int dw, dh;
	// SDL_GetWindowSizeInPixels(bd->Window, &dw, &dh);
	// if (w > 0 && h > 0)
	// 	io.DisplayFramebufferScale = ImVec2((float)dw / w, (float)dh / h);

	io.DeltaTime = ts;

	ImGui_ImplOpenGL3_NewFrame();
	{
		FBT_PROFILE_SCOPE("ImGui::NewFrame()");
		ImGui::NewFrame();
	}
}

void ImguiSubsystem::OnPostRender()
{
	FBT_PROFILE_FUNCTION();

	OnImguiRender.Broadcast();

	{
		FBT_PROFILE_SCOPE("ImGui::Render()");
		ImGui::Render();
	}

	ImDrawData* DrawData = ImGui::GetDrawData();
	ImGui_ImplOpenGL3_RenderDrawData(DrawData);
}

void ImguiSubsystem::Shutdown()
{
	FBT_PROFILE_FUNCTION();

	ImGuiIO& io = ImGui::GetIO();

	ImguiPlatformBackendData* bd = GetPlatformBackendData();
	io.BackendPlatformUserData = nullptr;
	io.BackendPlatformName = nullptr;
	delete bd;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void ImguiSubsystem::PreprocessWindowEvent(SDL_Event& event)
{
	ImGuiIO& io = ImGui::GetIO();

	switch (event.type) {
	case SDL_EVENT_MOUSE_MOTION:
		io.AddMousePosEvent(event.motion.x, event.motion.y);
		return;

	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_MOUSE_BUTTON_UP: {
		int button = -1;
		if (event.button.button == SDL_BUTTON_LEFT)
			button = 0;
		if (event.button.button == SDL_BUTTON_RIGHT)
			button = 1;
		if (event.button.button == SDL_BUTTON_MIDDLE)
			button = 2;
		if (button != -1)
			io.AddMouseButtonEvent(button, event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
		return;
	}

	case SDL_EVENT_MOUSE_WHEEL:
		io.AddMouseWheelEvent(event.wheel.x, event.wheel.y);
		return;

	case SDL_EVENT_KEY_DOWN:
	case SDL_EVENT_KEY_UP: {
		// Update modifier keys
		io.AddKeyEvent(ImGuiMod_Ctrl, (SDL_GetModState() & SDL_KMOD_CTRL) != 0);
		io.AddKeyEvent(ImGuiMod_Shift, (SDL_GetModState() & SDL_KMOD_SHIFT) != 0);
		io.AddKeyEvent(ImGuiMod_Alt, (SDL_GetModState() & SDL_KMOD_ALT) != 0);

		ImGuiKey key = SDL_ScancodeToImGuiKey(event.key.scancode);
		if (key != ImGuiKey_None)
			io.AddKeyEvent(key, event.type == SDL_EVENT_KEY_DOWN);
		return;
	}

	case SDL_EVENT_TEXT_INPUT:
		io.AddInputCharactersUTF8(event.text.text);
		return;

	case SDL_EVENT_WINDOW_FOCUS_GAINED:
		io.AddFocusEvent(true);
		return;

	case SDL_EVENT_WINDOW_FOCUS_LOST:
		io.AddFocusEvent(false);
		return;
	}
}

} // namespace Flibbert::Modules::Imgui
