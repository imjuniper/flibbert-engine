#include "Flibbert/Core/Input.h"

#include "Flibbert/Core/Application.h"
#include "Platform/Desktop/Window.h"

#include <rgfw/RGFW.h>

namespace Flibbert
{
	bool Input::IsKeyDown(KeyCode keycode)
	{
		RGFW_window* windowHandle = Application::Get().GetWindow()->GetNativeWindow();
		return RGFW_isPressed(windowHandle, (RGFW_key)keycode);
	}

	bool Input::IsKeyUp(KeyCode keycode)
	{
		RGFW_window* windowHandle = Application::Get().GetWindow()->GetNativeWindow();
		return RGFW_isReleased(windowHandle, (RGFW_key)keycode);
	}

	bool Input::IsMouseButtonDown(MouseButton button)
	{
		RGFW_window* windowHandle = Application::Get().GetWindow()->GetNativeWindow();
		return RGFW_isMousePressed(windowHandle, (RGFW_mouseButton)button);
	}

	glm::vec2 Input::GetMousePosition()
	{
		RGFW_window* windowHandle = Application::Get().GetWindow()->GetNativeWindow();
		RGFW_point mousePos = RGFW_window_getMousePoint(windowHandle);
		return glm::vec2{mousePos.x, mousePos.y};
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		RGFW_window* windowHandle = Application::Get().GetWindow()->GetNativeWindow();
		switch (mode) {
			case CursorMode::Normal:
				RGFW_window_mouseUnhold(windowHandle);
				break;
			case CursorMode::Hidden:
				RGFW_window_mouseUnhold(windowHandle);
				RGFW_window_showMouse(windowHandle, false);
				break;
			case CursorMode::Locked:
				RGFW_window_mouseHold(
				    windowHandle,
				    RGFW_AREA(windowHandle->r.w / 2, windowHandle->r.h / 2));
				RGFW_window_showMouse(windowHandle, false);
				break;
		}
	}
} // namespace Flibbert
