#include "Platform/Desktop/Window.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Input/Input.h"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_video.h"

namespace Flibbert {

Window::Window(const WindowProps& props)
{
	ZoneScoped;

	SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	constexpr SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	m_WindowHandle = SDL_CreateWindow(props.Title.c_str(), props.Width, props.Height, flags);

	int x, y;
	if (SDL_GetWindowPosition(m_WindowHandle, &x, &y)) {
		m_Position = {std::max(x, 0), std::max(y, 0)};
	}

	int w, h;
	if (SDL_GetWindowSize(m_WindowHandle, &w, &h)) {
		m_Size = {std::max(w, 0), std::max(h, 0)};
	}

	m_AspectRatio = static_cast<float>(m_Size.x) / m_Size.y;

	Input::Get().OnSetCursorMode.BindDynamic(this, Window::OnSetCursorMode);
}

Window::~Window()
{
	ZoneScoped;

	if (m_WindowHandle && SDL_WasInit(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		SDL_DestroyWindow(m_WindowHandle);
	}

	SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	SDL_Quit();
}

void Window::ProcessEvents()
{
	ZoneScoped;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		OnPreprocessEvent.Broadcast(event);

		switch (event.type) {
		case SDL_EVENT_QUIT: {
			OnWindowClosed.Broadcast(*this);
			break;
		}
		case SDL_EVENT_WINDOW_MOVED: {
			int x, y;
			if (SDL_GetWindowPosition(m_WindowHandle, &x, &y)) {
				m_Position = {std::max(x, 0), std::max(y, 0)};
			}
			OnWindowMoved.Broadcast(*this, m_Position);
			break;
		}
		case SDL_EVENT_WINDOW_RESIZED: {
			// @todo implement smooth resize
			int w, h;
			if (SDL_GetWindowSize(m_WindowHandle, &w, &h)) {
				m_Size = {std::max(w, 0), std::max(h, 0)};
			}
			m_AspectRatio = static_cast<float>(m_Size.x) / m_Size.y;
			OnWindowResized.Broadcast(*this, m_Size);
			break;
		}
		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN: {
			auto keyEvent = std::make_shared<InputEventKey>();
			keyEvent->Key = static_cast<Key>(event.key.key);
			keyEvent->IsPressed = event.key.down;
			Input::Get().ProcessInputEvent(keyEvent);
			Application::Get().DispatchInputEvent(keyEvent);
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_MOUSE_BUTTON_DOWN: {
			auto mouseButtonEvent = std::make_shared<InputEventMouseButton>();
			mouseButtonEvent->Position = glm::vec2{event.motion.x, event.motion.y};
			mouseButtonEvent->Button = static_cast<MouseButton>(event.button.button - 1);
			mouseButtonEvent->IsPressed = event.button.down;
			Input::Get().ProcessInputEvent(mouseButtonEvent);
			Application::Get().DispatchInputEvent(mouseButtonEvent);
			break;
		}
		case SDL_EVENT_MOUSE_MOTION: {
			auto mouseMovementEvent = std::make_shared<InputEventMouseMovement>();
			mouseMovementEvent->Position = glm::vec2{event.motion.x, event.motion.y};
			mouseMovementEvent->MovementDelta = glm::vec2{event.motion.xrel, event.motion.yrel};
			Input::Get().ProcessInputEvent(mouseMovementEvent);
			Application::Get().DispatchInputEvent(mouseMovementEvent);
			break;
		}
		default:
			// unhandled
			break;
		}
	}
}

void Window::Present()
{
	ZoneScoped;

	SDL_GL_SwapWindow(m_WindowHandle);
}

void Window::SetVSync(const bool enabled)
{
	ZoneScoped;

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
	ZoneScoped;

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
