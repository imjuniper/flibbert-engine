#pragma once

#include "Flibbert/Input/InputEvent.h"

#include <set>

namespace Flibbert
{
	class Input
	{
	public:
		using OnSetCursorModeDelegate = Delegate<void, CursorMode>;
		using InputEventDispatcherDelegate =
		    Delegate<void, const std::shared_ptr<InputEvent>&>;

		Input() = default;
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static Input& Get();

		void ProcessInputEvent(const std::shared_ptr<InputEvent>& event);

		[[nodiscard]] bool IsKeyPressed(Key key) const;
		[[nodiscard]] bool IsMouseButtonPressed(MouseButton button) const;
		[[nodiscard]] glm::vec2 GetMousePosition() const;

		[[nodiscard]] CursorMode GetCursorMode() const;
		void SetCursorMode(CursorMode mode);

	public:
		OnSetCursorModeDelegate OnSetCursorMode;
		InputEventDispatcherDelegate InputEventDispatch;

	private:
		static uint32_t GetMouseButtonAsMask(MouseButton button);

	private:
		std::set<Key> m_KeysPressed;
		uint8_t m_MouseButtonMask = 0; // @todo have a bitmask class like Godot?
		glm::vec2 m_MousePosition = {};
		CursorMode m_CursorMode = CursorMode::Normal;
	};
} // namespace Flibbert
