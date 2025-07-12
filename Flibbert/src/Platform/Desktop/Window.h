#pragma once

struct RGFW_window;

namespace Flibbert
{
	struct WindowProps {
		std::string Title = "Flibbert Engine";
		uint32_t Width = 960;
		uint32_t Height = 540;
	};

	class Window
	{
	public:
		explicit Window(const WindowProps& props = WindowProps());
		~Window();

		[[nodiscard]] RGFW_window* GetNativeWindow() const { return m_WindowHandle; }

		void SetWindowResizedCallback(const std::function<void(Window&)>& callback)
		{
			m_WindowResizedCallback = callback;
		}
		void OnWindowResize()
		{
			if (m_WindowResizedCallback)
				m_WindowResizedCallback(*this);
		}

	private:
		RGFW_window* m_WindowHandle = nullptr;

		std::function<void(Window&)> m_WindowResizedCallback;
	};
} // namespace Flibbert
