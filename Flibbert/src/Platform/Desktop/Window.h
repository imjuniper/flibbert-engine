#pragma once

struct RGFW_window;

namespace Flibbert
{
	struct WindowProps {
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		explicit WindowProps(const std::string& title = "Flibbert Engine",
		                     uint32_t width = 1600, uint32_t height = 900)
		    : Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		Window(const WindowProps& props = WindowProps());
		~Window();

		RGFW_window* GetNativeWindow() const { return m_WindowHandle; }

	private:
		RGFW_window* m_WindowHandle = nullptr;
	};
} // namespace Flibbert
