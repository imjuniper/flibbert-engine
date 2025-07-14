#include "Platform/Desktop/Window.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Renderer/Renderer.h"

#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#include <rgfw/RGFW.h>
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

		OnWindowMoved();
		OnWindowResized();
	}

	Window::~Window()
	{
		RGFW_window_close(m_WindowHandle);
	}

	glm::u32vec2 Window::GetSize() const
	{
		return m_Size;
	}

	glm::u32vec2 Window::GetPosition() const
	{
		return m_Position;
	}

	Window::OnWindowResizedDelegate Window::GetWindowResizedDelegate() const
	{
		return m_WindowResizedDelegate;
	}

	Window::OnWindowMovedDelegate Window::GetWindowMovedDelegate() const
	{
		return m_WindowMovedDelegate;
	}

	void Window::OnWindowResized()
	{
		m_Size = {std::max(m_WindowHandle->r.w, 0), std::max(m_WindowHandle->r.h, 0)};
		m_AspectRatio = static_cast<float>(m_Size.x) / m_Size.y;
		m_WindowResizedDelegate.Broadcast(*this, m_Size);
	}

	void Window::OnWindowMoved()
	{
		m_Position = {std::max(m_WindowHandle->r.x, 0), std::max(m_WindowHandle->r.y, 0)};
		m_WindowMovedDelegate.Broadcast(*this, m_Position);
	}
} // namespace Flibbert
