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

	private:
		RGFW_window* m_WindowHandle = nullptr;
	};
} // namespace Flibbert
