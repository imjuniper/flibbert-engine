#pragma once

#include "Flibbert/Core/KeyCodes.h"

#include <glm/glm.hpp>

namespace Flibbert
{

	class Input
	{
	public:
		static bool IsKeyDown(KeyCode keycode);
		static bool IsKeyUp(KeyCode keycode);
		static bool IsMouseButtonDown(MouseButton button);

		static glm::vec2 GetMousePosition();

		static void SetCursorMode(CursorMode mode);
	};

} // namespace Flibbert
