#include "Flibbert/Input/Input.h"

#include "Flibbert/Core/Application.h"

namespace Flibbert {

Input& Input::Get()
{
	static Input instance;
	return instance;
}

void Input::ProcessInputEvent(const std::shared_ptr<InputEvent>& event)
{
	FBT_PROFILE_FUNCTION();

	// @todo handle scroll wheel

	if (event->IsA<InputEventKey>()) {
		auto keyEvent = std::static_pointer_cast<InputEventKey>(event);

		if (keyEvent->IsPressed) {
			m_KeysPressed.insert(keyEvent->Key);
		}
		else {
			m_KeysPressed.erase(keyEvent->Key);
		}
	} else if (event->IsA<InputEventMouseButton>()) {
		auto mouseButtonEvent = std::static_pointer_cast<InputEventMouseButton>(event);

		m_MousePosition = mouseButtonEvent->Position;
		if (mouseButtonEvent->IsPressed) {
			m_MouseButtonMask |= GetMouseButtonAsMask(mouseButtonEvent->Button);
		}
		else {
			m_MouseButtonMask &= ~GetMouseButtonAsMask(mouseButtonEvent->Button);
		}
	} else if (event->IsA<InputEventMouseMovement>()) {
		auto mouseMoveEvent = std::static_pointer_cast<InputEventMouseMovement>(event);

		m_MousePosition = mouseMoveEvent->Position;
	}
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
	FBT_PROFILE_FUNCTION();

	if (m_CursorMode == mode)
		return;
	m_CursorMode = mode;
	OnSetCursorMode.ExecuteIfBound(mode);
}

uint32_t Input::GetMouseButtonAsMask(MouseButton button)
{
	return 1 << static_cast<int>(button);
}

} // namespace Flibbert
