#include "Input.h"

#include "Flibbert/Core/Application.h"
#include "Platform/Desktop/Window.h"

#include <rgfw/RGFW.h>

namespace Flibbert
{
	Input* Input::s_Instance = nullptr;

	Input& Input::Get()
	{
		if (!s_Instance) {
			s_Instance = new Input();
		}
		return *s_Instance;
	}

	void Input::ProcessInputEvent(const std::shared_ptr<InputEvent>& event)
	{
		auto keyEvent = dynamic_pointer_cast<InputEventKey>(event);
		if (keyEvent) {
			if (keyEvent->IsPressed) {
				m_KeysPressed.insert(keyEvent->Key);
			} else {
				m_KeysPressed.erase(keyEvent->Key);
			}
		}

		auto mouseButtonEvent = dynamic_pointer_cast<InputEventMouseButton>(event);
		if (mouseButtonEvent) {
			m_MousePosition = mouseButtonEvent->Position;
			if (mouseButtonEvent->IsPressed) {
				m_MouseButtonMask |= GetMouseButtonAsMask(mouseButtonEvent->Button);
			} else {
				m_MouseButtonMask &=
				    ~GetMouseButtonAsMask(mouseButtonEvent->Button);
			}
		}

		auto mouseMovementEvent = dynamic_pointer_cast<InputEventMouseMovement>(event);
		if (mouseMovementEvent) {
			m_MousePosition = mouseMovementEvent->Position;
		}

		InputEventDispatch.ExecuteIfBound(event);
	}

	bool Input::IsKeyPressed(Key key) const
	{
		return m_KeysPressed.contains(key);
	}

	bool Input::IsMouseButtonPressed(MouseButton button) const
	{
		return m_MouseButtonMask & (1 << static_cast<int>(button));
	}

	glm::vec2 Input::GetMousePosition() const
	{
		return m_MousePosition;
	}

	CursorMode Input::GetCursorMode() const
	{
		return m_CursorMode;
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		if (m_CursorMode == mode) return;
		m_CursorMode = mode;
		OnSetCursorMode.ExecuteIfBound(mode);
	}

	uint32_t Input::GetMouseButtonAsMask(MouseButton button)
	{
		return 1 << static_cast<int>(button);
	}

} // namespace Flibbert
