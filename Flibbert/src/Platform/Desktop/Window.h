#pragma once

#include "Flibbert/Core/Base.h"
#include "Flibbert/Input/InputEvent.h"

struct RGFW_window;

namespace Flibbert
{
	struct WindowProps {
		std::string Title = "Flibbert Engine";
		uint32_t Width = 960;
		uint32_t Height = 540;
	};

	// Needs to be the reparented probably
	class Window
	{
		using OnWindowResizedDelegate =
		    MulticastDelegate<Window&, const glm::u32vec2 /*size*/>;

		using OnWindowMovedDelegate =
		    MulticastDelegate<Window&, const glm::u32vec2 /*position*/>;

		using OnWindowClosedDelegate = MulticastDelegate<Window&>;

	public:
		explicit Window(const WindowProps& props = WindowProps());

		~Window();

		void InitImGui();
		void BeginImGuiFrame();
		void ShutdownImGui();

		void ProcessEvents();

		[[nodiscard]] glm::u32vec2 GetSize() const;
		[[nodiscard]] glm::u32vec2 GetPosition() const;
		[[nodiscard]] float GetAspectRatio() const { return m_AspectRatio; }

		[[nodiscard]] RGFW_window* GetNativeWindow() const { return m_WindowHandle; }

	public:
		OnWindowResizedDelegate OnWindowResized;
		OnWindowMovedDelegate OnWindowMoved;
		OnWindowClosedDelegate OnWindowClosed;

	private:
		void OnSetCursorMode(CursorMode mode);

	private:
		RGFW_window* m_WindowHandle = nullptr;

		glm::u32vec2 m_Position{0};
		glm::u32vec2 m_Size{0};
		float m_AspectRatio = 0.f;
	};
} // namespace Flibbert
