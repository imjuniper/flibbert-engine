#include "Platform/Desktop/Window.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Core/Log.h"
#include "Flibbert/Input/Input.h"
#include "Flibbert/Input/InputEvent.h"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_video.h"
#include "spdlog/common.h"

#include <memory>

namespace Flibbert {

MouseButton SDLMouseButtonToFlibbert(uint8_t button)
{
	switch (button) {
	case SDL_BUTTON_LEFT:
		return MouseButton::Left;
	case SDL_BUTTON_RIGHT:
		return MouseButton::Right;
	case SDL_BUTTON_MIDDLE:
		return MouseButton::Middle;
	case SDL_BUTTON_X1:
		return MouseButton::X1;
	case SDL_BUTTON_X2:
		return MouseButton::X2;
	}
}

Window::Window(const WindowProps& props)
{
	FBT_PROFILE_FUNCTION();

	SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	constexpr SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	m_WindowHandle = SDL_CreateWindow(props.Title.c_str(), props.Width, props.Height, flags);
	// @todo only start it when necessary
	SDL_StartTextInput(m_WindowHandle);

	int x, y;
	if (SDL_GetWindowPosition(m_WindowHandle, &x, &y)) {
		m_Position = {std::max(x, 0), std::max(y, 0)};
	}

	int w, h;
	if (SDL_GetWindowSize(m_WindowHandle, &w, &h)) {
		m_Size = {std::max(w, 0), std::max(h, 0)};
	}

	m_AspectRatio = static_cast<float>(m_Size.x) / m_Size.y;

	Input::Get().OnSetCursorMode.Bind<&Window::OnSetCursorMode>(this);
}

Window::~Window()
{
	FBT_PROFILE_FUNCTION();

	if (m_WindowHandle && SDL_WasInit(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		SDL_StopTextInput(m_WindowHandle);
		SDL_DestroyWindow(m_WindowHandle);
	}

	SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	SDL_Quit();
}

void Window::ProcessEvents()
{
	FBT_PROFILE_FUNCTION();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		std::shared_ptr<InputEvent> eventToDispatch;

		switch (event.type) {
		/// Window Events ///
		case SDL_EVENT_QUIT: {
			OnWindowClosed.Broadcast(*this);
			return;
		}
		case SDL_EVENT_WINDOW_MOVED: {
			int x, y;
			if (SDL_GetWindowPosition(m_WindowHandle, &x, &y)) {
				m_Position = {std::max(x, 0), std::max(y, 0)};
			}
			OnWindowMoved.Broadcast(*this, m_Position);
			return;
		}
		case SDL_EVENT_WINDOW_RESIZED: {
			// @todo implement smooth resize
			int w, h;
			if (SDL_GetWindowSize(m_WindowHandle, &w, &h)) {
				m_Size = {std::max(w, 0), std::max(h, 0)};
			}
			m_AspectRatio = static_cast<float>(m_Size.x) / m_Size.y;
			OnWindowResized.Broadcast(*this, m_Size);
			return;
		}
		case SDL_EVENT_WINDOW_FOCUS_GAINED: {
			OnWindowFocusLost.Broadcast(*this);
			return;
		}
		case SDL_EVENT_WINDOW_FOCUS_LOST: {
			OnWindowFocusLost.Broadcast(*this);
			return;
		}

		/// Input Events ///
		case SDL_EVENT_TEXT_INPUT: {
			auto textEvent = std::make_shared<InputEventText>();
			textEvent->Text = event.text.text;
			textEvent->CtrlPressed = (SDL_GetModState() & SDL_KMOD_CTRL) != 0;
			textEvent->ShiftPressed = (SDL_GetModState() & SDL_KMOD_SHIFT) != 0;
			textEvent->AltPressed = (SDL_GetModState() & SDL_KMOD_ALT) != 0;
			eventToDispatch = textEvent;
			break;
		}
		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN: {
			auto keyEvent = std::make_shared<InputEventKey>();
			keyEvent->Key = static_cast<Key>(event.key.key);
			keyEvent->IsPressed = event.key.down;
			keyEvent->CtrlPressed = (SDL_GetModState() & SDL_KMOD_CTRL) != 0;
			keyEvent->ShiftPressed = (SDL_GetModState() & SDL_KMOD_SHIFT) != 0;
			keyEvent->AltPressed = (SDL_GetModState() & SDL_KMOD_ALT) != 0;
			eventToDispatch = keyEvent;
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_MOUSE_BUTTON_DOWN: {
			auto mouseButtonEvent = std::make_shared<InputEventMouseButton>();
			mouseButtonEvent->Position = glm::vec2{event.button.x, event.button.y};
			mouseButtonEvent->Button = SDLMouseButtonToFlibbert(event.button.button);
			mouseButtonEvent->IsPressed = event.button.down;
			mouseButtonEvent->CtrlPressed = (SDL_GetModState() & SDL_KMOD_CTRL) != 0;
			mouseButtonEvent->ShiftPressed = (SDL_GetModState() & SDL_KMOD_SHIFT) != 0;
			mouseButtonEvent->AltPressed = (SDL_GetModState() & SDL_KMOD_ALT) != 0;
			eventToDispatch = mouseButtonEvent;
			break;
		}
		case SDL_EVENT_MOUSE_WHEEL:{
			auto mouseWheelEvent = std::make_shared<InputEventMouseWheel>();
			mouseWheelEvent->Position = glm::vec2{event.wheel.mouse_x, event.wheel.mouse_y};
			mouseWheelEvent->Amount = glm::vec2{event.wheel.x, event.wheel.y};
			mouseWheelEvent->CtrlPressed = (SDL_GetModState() & SDL_KMOD_CTRL) != 0;
			mouseWheelEvent->ShiftPressed = (SDL_GetModState() & SDL_KMOD_SHIFT) != 0;
			mouseWheelEvent->AltPressed = (SDL_GetModState() & SDL_KMOD_ALT) != 0;
			eventToDispatch = mouseWheelEvent;
			break;
		}
		case SDL_EVENT_MOUSE_MOTION: {
			auto mouseMovementEvent = std::make_shared<InputEventMouseMovement>();
			mouseMovementEvent->Position = glm::vec2{event.motion.x, event.motion.y};
			mouseMovementEvent->MovementDelta = glm::vec2{event.motion.xrel, event.motion.yrel};
			mouseMovementEvent->CtrlPressed = (SDL_GetModState() & SDL_KMOD_CTRL) != 0;
			mouseMovementEvent->ShiftPressed = (SDL_GetModState() & SDL_KMOD_SHIFT) != 0;
			mouseMovementEvent->AltPressed = (SDL_GetModState() & SDL_KMOD_ALT) != 0;
			eventToDispatch = mouseMovementEvent;
			break;
		}
		default:
			// unhandled
			return;
		}

		// @todo Refactor later into a loop to be able to consume events
		OnPreprocessEvent.Broadcast(eventToDispatch);
		Input::Get().ProcessInputEvent(eventToDispatch);
		Application::Get().DispatchInputEvent(eventToDispatch);
	}
}

void Window::Present()
{
	FBT_PROFILE_FUNCTION();

	SDL_GL_SwapWindow(m_WindowHandle);
}

void Window::SetVSync(const bool enabled)
{
	FBT_PROFILE_FUNCTION();

	m_VSync = enabled;
	SDL_GL_SetSwapInterval(static_cast<int>(m_VSync));
}

bool Window::IsVSyncEnabled() const
{
	return m_VSync;
}

glm::u32vec2 Window::GetSize() const
{
	return m_Size;
}

glm::u32vec2 Window::GetPosition() const
{
	return m_Position;
}

void Window::OnSetCursorMode(CursorMode mode)
{
	FBT_PROFILE_FUNCTION();

	FBT_CORE_LOG(info, "wow!");

	switch (mode) {
	case CursorMode::Normal:
		SDL_CaptureMouse(false);
		SDL_ShowCursor();
		break;
	case CursorMode::Hidden:
		SDL_CaptureMouse(false);
		SDL_HideCursor();
		break;
	case CursorMode::Locked:
		SDL_CaptureMouse(true);
		SDL_HideCursor();
		break;
	}
}

} // namespace Flibbert
