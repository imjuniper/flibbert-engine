#pragma once

struct RGFW_window;

namespace Flibbert
{

	class Window
	{

	public:
		Window();
		~Window();

		RGFW_window* GetNativeWindow() const { return m_Window; }

	private:
		RGFW_window* m_Window = nullptr;
	};

} // namespace Flibbert
