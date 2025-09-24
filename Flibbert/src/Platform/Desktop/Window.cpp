#include "Platform/Desktop/Window.h"

#include "Flibbert/Input/Input.h"
#include "Flibbert/Renderer/Renderer.h"

#ifdef FBT_DEBUG
	#define RGFW_DEBUG
#endif
#define RGFW_NATIVE
#define RGFW_IMPLEMENTATION
#define RGFW_IMGUI_IMPLEMENTATION
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#include <rgfw/imgui_impl_rgfw.h>
#pragma clang diagnostic pop

namespace Flibbert
{
	Window::Window(const WindowProps& props)
	{
		ZoneScoped;

		m_WindowHandle = RGFW_createWindow(props.Title.c_str(),
		                                   0, 0, props.Width, props.Height,
		                                   RGFW_windowCenter);

		m_Position = {std::max(m_WindowHandle->x, 0), std::max(m_WindowHandle->y, 0)};
		m_Size = {std::max(m_WindowHandle->w, 0), std::max(m_WindowHandle->h, 0)};
		m_AspectRatio = static_cast<float>(m_Size.x) / m_Size.y;

		Input::Get().OnSetCursorMode.BindDynamic(this, Window::OnSetCursorMode);
	}

	Window::~Window()
	{
		ZoneScoped;

		RGFW_window_close(m_WindowHandle);
	}

	void Window::InitImGui()
	{
		ZoneScoped;

		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				ImGui_ImplRgfw_InitForOpenGL(GetNativeWindow(), true);
				break;
			default:
				ImGui_ImplRgfw_InitForOther(GetNativeWindow(), true);
				break;

		}

		auto scale = RGFW_window_getMonitor(m_WindowHandle).pixelRatio;
		ImGui::GetIO().DisplayFramebufferScale = {scale, scale};
	}

	void Window::BeginImGuiFrame()
	{
		ZoneScoped;

		ImGui_ImplRgfw_NewFrame();
	}

	void Window::ShutdownImGui()
	{
		ZoneScoped;

		ImGui_ImplRgfw_Shutdown();
	}

	void Window::ProcessEvents()
	{
		ZoneScoped;

		RGFW_event event;
		while (RGFW_window_checkEvent(m_WindowHandle, &event)) {
			switch (event.type) {
				case RGFW_quit: {
					OnWindowClosed.Broadcast(*this);
					break;
				}
				case RGFW_windowMoved: {
					m_Position = {std::max(m_WindowHandle->x, 0),
						      std::max(m_WindowHandle->y, 0)};
					OnWindowMoved.Broadcast(*this, m_Position);
					break;
				}
				case RGFW_windowResized: {
					// @todo implement smooth resize
					// https://github.com/ColleagueRiley/RGFW/blob/main/examples/smooth-resize/smooth-resize.c?rgh-link-date=2025-06-23T16%3A00%3A55.000Z
					m_Size = {std::max(m_WindowHandle->w, 0),
						  std::max(m_WindowHandle->h, 0)};
					m_AspectRatio = static_cast<float>(m_Size.x) / m_Size.y;
					OnWindowResized.Broadcast(*this, m_Size);
					break;
				}
				case RGFW_keyPressed: {
					auto keyEvent = std::make_shared<InputEventKey>();
					keyEvent->Key = static_cast<Key>(event.key.value);
					keyEvent->IsPressed = true;
					Input::Get().ProcessInputEvent(keyEvent);
					break;
				}
				case RGFW_keyReleased: {
					auto keyEvent = std::make_shared<InputEventKey>();
					keyEvent->Key = static_cast<Key>(event.key.value);
					keyEvent->IsPressed = false;
					Input::Get().ProcessInputEvent(keyEvent);
					break;
				}
				case RGFW_mouseButtonPressed: {
					auto mouseButtonEvent =
					    std::make_shared<InputEventMouseButton>();
					mouseButtonEvent->Position =
					    glm::vec2{event.mouse.x, event.mouse.y};
					mouseButtonEvent->Button =
					    static_cast<MouseButton>(event.button.value);
					mouseButtonEvent->IsPressed = true;
					Input::Get().ProcessInputEvent(mouseButtonEvent);
					break;
				}
				case RGFW_mouseButtonReleased: {
					auto mouseButtonEvent =
					    std::make_shared<InputEventMouseButton>();
					mouseButtonEvent->Position =
					    glm::vec2{event.mouse.x, event.mouse.y};
					mouseButtonEvent->Button =
					    static_cast<MouseButton>(event.button.value);
					mouseButtonEvent->IsPressed = false;
					Input::Get().ProcessInputEvent(mouseButtonEvent);
					break;
				}
				case RGFW_mousePosChanged: {
					auto mouseMovementEvent =
					    std::make_shared<InputEventMouseMovement>();
					mouseMovementEvent->Position =
					    glm::vec2{event.mouse.x, event.mouse.y};
					mouseMovementEvent->MovementDelta =
					    glm::vec2{event.mouse.vecX, event.mouse.vecY};
					Input::Get().ProcessInputEvent(mouseMovementEvent);
					break;
				}
				default:
					// unhandled
					break;
			}
		}
	}

	void Window::SwapBuffers()
	{
		ZoneScoped;

		// @todo probably implement this manually?
		//RGFW_window_swapBuffers_OpenGL(m_WindowHandle);
	}

	void Window::SetVSync(const bool enabled)
	{
		ZoneScoped;

		m_VSync = enabled;
		//RGFW_window_swapInterval_OpenGL(m_WindowHandle, m_VSync);
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
				RGFW_window_unholdMouse(m_WindowHandle);
				RGFW_window_showMouse(m_WindowHandle, true);
				break;
			case CursorMode::Hidden:
				RGFW_window_unholdMouse(m_WindowHandle);
				RGFW_window_showMouse(m_WindowHandle, false);
				break;
			case CursorMode::Locked:
				RGFW_window_holdMouse(m_WindowHandle);
				RGFW_window_showMouse(m_WindowHandle, false);
				break;
		}
	}
} // namespace Flibbert
