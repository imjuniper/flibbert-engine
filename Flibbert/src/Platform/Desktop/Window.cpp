#include "Platform/Desktop/Window.h"

#include "Flibbert/Input/Input.h"
#include "Flibbert/Renderer/Renderer.h"

#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION
#define RGFW_IMGUI_IMPLEMENTATION
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#include <rgfw/imgui_impl_rgfw.h> // Also includes RGFW.h
#pragma clang diagnostic pop

namespace Flibbert
{
	Window::Window(const WindowProps& props)
	{
		// @todo check if this is needed on other platforms than macOS
		if (Renderer::GetAPI() == Renderer::API::OpenGL) {
			RGFW_setGLHint(RGFW_glDebug, 1);
			RGFW_setGLHint(RGFW_glMajor, 4);
#ifdef FBT_PLATFORM_MACOS
			/* Set OpenGL version to 4.1 on macOS */
			RGFW_setGLHint(RGFW_glMinor, 1);
#else
			RGFW_setGLHint(RGFW_glMinor, 6);
#endif
			RGFW_setGLHint(RGFW_glProfile, RGFW_glCore);
		}

		m_WindowHandle = RGFW_createWindow(props.Title.c_str(),
		                                   RGFW_RECT(0, 0, props.Width, props.Height),
		                                   RGFW_windowCenter);

		m_Position = {std::max(m_WindowHandle->r.x, 0), std::max(m_WindowHandle->r.y, 0)};
		m_Size = {std::max(m_WindowHandle->r.w, 0), std::max(m_WindowHandle->r.h, 0)};
		m_AspectRatio = static_cast<float>(m_Size.x) / m_Size.y;

		Input::Get().OnSetCursorMode.Bind(FBT_BIND_EVENT(Window::OnSetCursorMode));
	}

	Window::~Window()
	{
		RGFW_window_close(m_WindowHandle);
	}

	void Window::InitImGui()
	{
		ImGui_ImplRgfw_InitForOpenGL(GetNativeWindow(), true);
	}

	void Window::BeginImGuiFrame()
	{
		ImGui_ImplRgfw_NewFrame();
	}

	void Window::ShutdownImGui()
	{
		ImGui_ImplRgfw_Shutdown();
	}

	void Window::ProcessEvents()
	{
		while (RGFW_window_checkEvent(m_WindowHandle)) {
			RGFW_event& event = m_WindowHandle->event;

			switch (event.type) {
				case RGFW_quit:
					OnWindowClosed.Broadcast(*this);
					break;

				case RGFW_windowMoved:
					m_Position = {std::max(m_WindowHandle->r.x, 0),
					              std::max(m_WindowHandle->r.y, 0)};
					OnWindowMoved.Broadcast(*this, m_Position);
					break;

				case RGFW_windowResized:
					// @todo implement smooth resize
					// https://github.com/ColleagueRiley/RGFW/blob/main/examples/smooth-resize/smooth-resize.c?rgh-link-date=2025-06-23T16%3A00%3A55.000Z
					m_Size = {std::max(m_WindowHandle->r.w, 0),
					          std::max(m_WindowHandle->r.h, 0)};
					m_AspectRatio = static_cast<float>(m_Size.x) / m_Size.y;
					OnWindowResized.Broadcast(*this, m_Size);
					break;

				case RGFW_keyPressed: {
					auto keyEvent = std::make_shared<InputEventKey>();
					keyEvent->Key = static_cast<Key>(event.key);
					keyEvent->IsPressed = true;
					Input::Get().ProcessInputEvent(keyEvent);
					break;
				}
				case RGFW_keyReleased: {
					auto keyEvent = std::make_shared<InputEventKey>();
					keyEvent->Key = static_cast<Key>(event.key);
					keyEvent->IsPressed = false;
					Input::Get().ProcessInputEvent(keyEvent);
					break;
				}

				case RGFW_mouseButtonPressed: {
					auto mouseButtonEvent =
					    std::make_shared<InputEventMouseButton>();
					mouseButtonEvent->Position =
					    glm::vec2{event.point.x, event.point.y};
					mouseButtonEvent->Button =
					    static_cast<MouseButton>(event.button);
					mouseButtonEvent->IsPressed = true;
					Input::Get().ProcessInputEvent(mouseButtonEvent);
					break;
				}
				case RGFW_mouseButtonReleased: {
					auto mouseButtonEvent =
					    std::make_shared<InputEventMouseButton>();
					mouseButtonEvent->Position =
					    glm::vec2{event.point.x, event.point.y};
					mouseButtonEvent->Button =
					    static_cast<MouseButton>(event.button);
					mouseButtonEvent->IsPressed = false;
					Input::Get().ProcessInputEvent(mouseButtonEvent);
					break;
				}
				case RGFW_mousePosChanged: {
					auto mouseMovementEvent =
					    std::make_shared<InputEventMouseMovement>();
					mouseMovementEvent->Position =
					    glm::vec2{event.point.x, event.point.y};
					mouseMovementEvent->MovementDelta =
					    glm::vec2{event.vector.x, event.vector.y};
					Input::Get().ProcessInputEvent(mouseMovementEvent);
					break;
				}

				default:
					break;
			}
		}
	}

	void Window::SwapBuffers()
	{
		RGFW_window_swapBuffers(m_WindowHandle);
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
		switch (mode) {
			case CursorMode::Normal:
				RGFW_window_mouseUnhold(m_WindowHandle);
				RGFW_window_showMouse(m_WindowHandle, true);
				break;
			case CursorMode::Hidden:
				RGFW_window_mouseUnhold(m_WindowHandle);
				RGFW_window_showMouse(m_WindowHandle, false);
				break;
			case CursorMode::Locked:
				RGFW_window_mouseHold(
				    m_WindowHandle,
				    RGFW_AREA(m_WindowHandle->r.w / 2, m_WindowHandle->r.h / 2));
				RGFW_window_showMouse(m_WindowHandle, false);
				break;
		}
	}
} // namespace Flibbert
