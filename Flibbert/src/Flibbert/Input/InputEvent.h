#pragma once

namespace Flibbert
{
	enum class Key {
		None = 0,
		Escape = '\033',
		Backtick = '`',
		Num0 = '0',
		Num1 = '1',
		Num2 = '2',
		Num3 = '3',
		Num4 = '4',
		Num5 = '5',
		Num6 = '6',
		Num7 = '7',
		Num8 = '8',
		Num9 = '9',

		Minus = '-',
		Equals = '=',
		BackSpace = '\b',
		Tab = '\t',
		Space = ' ',

		A = 'a',
		B = 'b',
		C = 'c',
		D = 'd',
		E = 'e',
		F = 'f',
		G = 'g',
		H = 'h',
		I = 'i',
		J = 'j',
		K = 'k',
		L = 'l',
		M = 'm',
		N = 'n',
		O = 'o',
		P = 'p',
		Q = 'q',
		R = 'r',
		S = 's',
		T = 't',
		U = 'u',
		V = 'v',
		W = 'w',
		X = 'x',
		Y = 'y',
		Z = 'z',

		Period = '.',
		Comma = ',',
		Slash = '/',
		Bracket = '[',
		CloseBracket = ']',
		Semicolon = ';',
		Apostrophe = '\'',
		BackSlash = '\\',
		Return = '\n',
		Enter = Return,

		Delete = '\177', /* 127 */

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		CapsLock,
		LeftShift,
		LeftControl,
		LeftAlt,
		LeftSuper,
		RightShift,
		RightControl,
		RightAlt,
		RightSuper,
		ArrowUp,
		ArrowDown,
		ArrowLeft,
		ArrowRight,
		Insert,
		End,
		Home,
		PageUp,
		PageDown,

		NumLock,
		NumpadSlash,
		NumpadMultiply,
		NumpadMinus,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		Numpad0,
		NumpadPeriod,
		NumpadReturn,
		ScrollLock,
		PrintScreen,
		Pause,
	};

	enum class CursorMode { Normal = 0, Hidden = 1, Locked = 2 };

	enum class MouseButton { Left = 0, Middle = 1, Right = 2, ScrollUp = 3, ScrollDown = 4 };

	struct InputEvent {
		virtual ~InputEvent() = default;
	};

	struct InputEventKey final : InputEvent {
		Key Key;
		bool IsPressed = false;
	};

	struct InputEventMouse : InputEvent {
		glm::vec2 Position;
	};

	struct InputEventMouseButton final : InputEventMouse {
		MouseButton Button;
		bool IsPressed = false;
	};

	struct InputEventMouseMovement final : InputEventMouse {
		glm::vec2 MovementDelta;
	};
} // namespace Flibbert
