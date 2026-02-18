#pragma once

#include "Flibbert/Core/Base.h"
#include "Flibbert/Input/InputEvent.h"

struct SDL_Window;
typedef union SDL_Event SDL_Event;

namespace Flibbert {

struct WindowProps
{
	std::string Title = "Flibbert Engine";
	uint32_t Width = 960;
	uint32_t Height = 540;
};

// Needs to be the reparented probably
class Window
{
	using OnWindowResizedDelegate = MulticastDelegate<Window&, const glm::u32vec2 /*size*/>;

	using OnWindowMovedDelegate = MulticastDelegate<Window&, const glm::u32vec2 /*position*/>;

	using OnWindowClosedDelegate = MulticastDelegate<Window&>;

	// @todo handle all events with my own type
	using OnPreprocessEventDelegate = MulticastDelegate<SDL_Event&>;

public:
	explicit Window(const WindowProps& props = WindowProps());

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	~Window();

	void ProcessEvents();
	void Present();

	void SetVSync(bool enabled);
	bool IsVSyncEnabled() const;

	[[nodiscard]] glm::u32vec2 GetSize() const;
	[[nodiscard]] glm::u32vec2 GetPosition() const;
	[[nodiscard]] float GetAspectRatio() const
	{
		return m_AspectRatio;
	}

	[[nodiscard]] SDL_Window* GetNativeWindow() const
	{
		return m_WindowHandle;
	}

public:
	OnWindowResizedDelegate OnWindowResized;
	OnWindowMovedDelegate OnWindowMoved;
	OnWindowClosedDelegate OnWindowClosed;
	OnPreprocessEventDelegate OnPreprocessEvent;

private:
	void OnSetCursorMode(CursorMode mode);

private:
	bool m_VSync = true;
	SDL_Window* m_WindowHandle = nullptr;

	glm::u32vec2 m_Position{0};
	glm::u32vec2 m_Size{0};
	float m_AspectRatio = 0.f;
};

} // namespace Flibbert
