#include "ImguiSubsystem.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Input/InputEvent.h"

#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

#include <memory>

namespace Flibbert::Modules::Imgui {

static ImGuiKey FlibbertKeyToImGuiKey(Key key)
{
	switch (key) {
	case Key::Space:
		return ImGuiKey_Space;
	case Key::Apostrophe:
		return ImGuiKey_Apostrophe;
	case Key::Comma:
		return ImGuiKey_Comma;
	case Key::Minus:
		return ImGuiKey_Minus;
	case Key::Period:
		return ImGuiKey_Period;
	case Key::Slash:
		return ImGuiKey_Slash;
	case Key::Semicolon:
		return ImGuiKey_Semicolon;
	case Key::Equals:
		return ImGuiKey_Equal;
	case Key::Bracket:
		return ImGuiKey_LeftBracket;
	case Key::CloseBracket:
		return ImGuiKey_RightBracket;
	case Key::BackSlash:
		return ImGuiKey_Backslash;
	case Key::Backtick:
		return ImGuiKey_GraveAccent;

	case Key::Num0:
		return ImGuiKey_0;
	case Key::Num1:
		return ImGuiKey_1;
	case Key::Num2:
		return ImGuiKey_2;
	case Key::Num3:
		return ImGuiKey_3;
	case Key::Num4:
		return ImGuiKey_4;
	case Key::Num5:
		return ImGuiKey_5;
	case Key::Num6:
		return ImGuiKey_6;
	case Key::Num7:
		return ImGuiKey_7;
	case Key::Num8:
		return ImGuiKey_8;
	case Key::Num9:
		return ImGuiKey_9;

	case Key::A:
		return ImGuiKey_A;
	case Key::B:
		return ImGuiKey_B;
	case Key::C:
		return ImGuiKey_C;
	case Key::D:
		return ImGuiKey_D;
	case Key::E:
		return ImGuiKey_E;
	case Key::F:
		return ImGuiKey_F;
	case Key::G:
		return ImGuiKey_G;
	case Key::H:
		return ImGuiKey_H;
	case Key::I:
		return ImGuiKey_I;
	case Key::J:
		return ImGuiKey_J;
	case Key::K:
		return ImGuiKey_K;
	case Key::L:
		return ImGuiKey_L;
	case Key::M:
		return ImGuiKey_M;
	case Key::N:
		return ImGuiKey_N;
	case Key::O:
		return ImGuiKey_O;
	case Key::P:
		return ImGuiKey_P;
	case Key::Q:
		return ImGuiKey_Q;
	case Key::R:
		return ImGuiKey_R;
	case Key::S:
		return ImGuiKey_S;
	case Key::T:
		return ImGuiKey_T;
	case Key::U:
		return ImGuiKey_U;
	case Key::V:
		return ImGuiKey_V;
	case Key::W:
		return ImGuiKey_W;
	case Key::X:
		return ImGuiKey_X;
	case Key::Y:
		return ImGuiKey_Y;
	case Key::Z:
		return ImGuiKey_Z;

	// Control keys
	case Key::Tab:
		return ImGuiKey_Tab;
	case Key::BackSpace:
		return ImGuiKey_Backspace;
	case Key::Return:
		return ImGuiKey_Enter;
	case Key::Escape:
		return ImGuiKey_Escape;
	case Key::Delete:
		return ImGuiKey_Delete;
	case Key::Insert:
		return ImGuiKey_Insert;
	case Key::Home:
		return ImGuiKey_Home;
	case Key::End:
		return ImGuiKey_End;
	case Key::PageUp:
		return ImGuiKey_PageUp;
	case Key::PageDown:
		return ImGuiKey_PageDown;

	case Key::ArrowUp:
		return ImGuiKey_UpArrow;
	case Key::ArrowDown:
		return ImGuiKey_DownArrow;
	case Key::ArrowLeft:
		return ImGuiKey_LeftArrow;
	case Key::ArrowRight:
		return ImGuiKey_RightArrow;

	// Modifiers
	case Key::LeftShift:
		return ImGuiKey_LeftShift;
	case Key::RightShift:
		return ImGuiKey_RightShift;
	case Key::LeftControl:
		return ImGuiKey_LeftCtrl;
	case Key::RightControl:
		return ImGuiKey_RightCtrl;
	case Key::LeftAlt:
		return ImGuiKey_LeftAlt;
	case Key::RightAlt:
		return ImGuiKey_RightAlt;
	case Key::LeftSuper:
		return ImGuiKey_LeftSuper;
	case Key::RightSuper:
		return ImGuiKey_RightSuper;
	case Key::CapsLock:
		return ImGuiKey_CapsLock;
	case Key::ScrollLock:
		return ImGuiKey_ScrollLock;
	case Key::NumLock:
		return ImGuiKey_NumLock;
	case Key::PrintScreen:
		return ImGuiKey_PrintScreen;
	case Key::Pause:
		return ImGuiKey_Pause;

	// Function keys
	case Key::F1:
		return ImGuiKey_F1;
	case Key::F2:
		return ImGuiKey_F2;
	case Key::F3:
		return ImGuiKey_F3;
	case Key::F4:
		return ImGuiKey_F4;
	case Key::F5:
		return ImGuiKey_F5;
	case Key::F6:
		return ImGuiKey_F6;
	case Key::F7:
		return ImGuiKey_F7;
	case Key::F8:
		return ImGuiKey_F8;
	case Key::F9:
		return ImGuiKey_F9;
	case Key::F10:
		return ImGuiKey_F10;
	case Key::F11:
		return ImGuiKey_F11;
	case Key::F12:
		return ImGuiKey_F12;

	// Numpad
	case Key::Numpad0:
		return ImGuiKey_Keypad0;
	case Key::Numpad1:
		return ImGuiKey_Keypad1;
	case Key::Numpad2:
		return ImGuiKey_Keypad2;
	case Key::Numpad3:
		return ImGuiKey_Keypad3;
	case Key::Numpad4:
		return ImGuiKey_Keypad4;
	case Key::Numpad5:
		return ImGuiKey_Keypad5;
	case Key::Numpad6:
		return ImGuiKey_Keypad6;
	case Key::Numpad7:
		return ImGuiKey_Keypad7;
	case Key::Numpad8:
		return ImGuiKey_Keypad8;
	case Key::Numpad9:
		return ImGuiKey_Keypad9;
	case Key::NumpadPeriod:
		return ImGuiKey_KeypadDecimal;
	case Key::NumpadSlash:
		return ImGuiKey_KeypadDivide;
	case Key::NumpadMultiply:
		return ImGuiKey_KeypadMultiply;
	case Key::NumpadMinus:
		return ImGuiKey_KeypadSubtract;
	case Key::NumpadReturn:
		return ImGuiKey_KeypadEnter;

	default:
		return ImGuiKey_None;
	}
}

struct ImguiPlatformBackendData
{
	Window* Window;
	uint64_t LastTime;
};

static ImguiPlatformBackendData* GetPlatformBackendData()
{
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

	(void)application->GetWindow().OnPreprocessEvent.AddDynamic(this, ThisClass::PreprocessInputEvent);
	(void)application->GetWindow().OnWindowFocusGained.AddDynamic(this, ThisClass::HandleWindowFocusedGained);
	(void)application->GetWindow().OnWindowFocusLost.AddDynamic(this, ThisClass::HandleWindowFocusedLost);
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

void ImguiSubsystem::PreprocessInputEvent(const std::shared_ptr<InputEvent>& event)
{
	ImGuiIO& io = ImGui::GetIO();

	if (event->IsA<InputEventWithModifier>()) {
		auto eventWithMods = std::static_pointer_cast<InputEventWithModifier>(event);
		io.AddKeyEvent(ImGuiMod_Ctrl, eventWithMods->CtrlPressed);
		io.AddKeyEvent(ImGuiMod_Shift, eventWithMods->ShiftPressed);
		io.AddKeyEvent(ImGuiMod_Alt, eventWithMods->AltPressed);
		io.AddKeyEvent(ImGuiMod_Super, eventWithMods->SuperPressed);
	}

	if (event->IsA<InputEventText>()) {
		auto textEvent = std::static_pointer_cast<InputEventText>(event);
		io.AddInputCharactersUTF8(textEvent->Text);
	} else if (event->IsA<InputEventKey>()) {
		auto keyEvent = std::static_pointer_cast<InputEventKey>(event);
		ImGuiKey key = FlibbertKeyToImGuiKey(keyEvent->Key);
		if (key != ImGuiKey_None) {
			io.AddKeyEvent(key, keyEvent->IsPressed);
		}
	} else if (event->IsA<InputEventMouseButton>()) {
		auto mouseButtonEvent = std::static_pointer_cast<InputEventMouseButton>(event);
		io.AddMouseButtonEvent(static_cast<int32_t>(mouseButtonEvent->Button), mouseButtonEvent->IsPressed);
	} else if (event->IsA<InputEventMouseWheel>()) {
		auto mouseWheelEvent = std::static_pointer_cast<InputEventMouseWheel>(event);
		io.AddMouseWheelEvent(mouseWheelEvent->Amount.x, mouseWheelEvent->Amount.y);
	} else if (event->IsA<InputEventMouseMovement>()) {
		auto mouseMoveEvent = std::static_pointer_cast<InputEventMouseMovement>(event);
		io.AddMousePosEvent(mouseMoveEvent->Position.x, mouseMoveEvent->Position.y);
	}
}

void ImguiSubsystem::HandleWindowFocusedGained(Window& window)
{
	ImGui::GetIO().AddFocusEvent(true);
}

void ImguiSubsystem::HandleWindowFocusedLost(Window& window)
{
	ImGui::GetIO().AddFocusEvent(false);
}

} // namespace Flibbert::Modules::Imgui
