#pragma once

#include "Flibbert/Core/Base.h"

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
		// is it worth passing by const ref?
		using OnWindowResizedDelegate =
		    MulticastDelegate<Window& /*window*/, const glm::u32vec2& /*size*/>;
		using OnWindowMovedDelegate =
		    MulticastDelegate<Window& /*window*/, const glm::u32vec2& /*position*/>;

	public:
		explicit Window(const WindowProps& props = WindowProps());

		~Window();

		[[nodiscard]] glm::u32vec2 GetSize() const;
		[[nodiscard]] glm::u32vec2 GetPosition() const;
		[[nodiscard]] float GetAspectRatio() const { return m_AspectRatio; }

		[[nodiscard]] RGFW_window* GetNativeWindow() const { return m_WindowHandle; }

		OnWindowResizedDelegate GetWindowResizedDelegate() const;

		OnWindowResizedDelegate GetWindowMovedDelegate() const;

	private:
		friend class Application;

		void OnWindowResized();
		void OnWindowMoved();

	private:
		RGFW_window* m_WindowHandle = nullptr;

		glm::u32vec2 m_Position;
		glm::u32vec2 m_Size;
		float m_AspectRatio;

		OnWindowResizedDelegate m_WindowResizedDelegate;
		OnWindowMovedDelegate m_WindowMovedDelegate;
	};
} // namespace Flibbert
