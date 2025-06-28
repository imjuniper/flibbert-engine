#pragma once

struct RGFW_window;

namespace Flibbert
{

	class Application
	{
	public:
		Application();
		virtual ~Application();

		static Application& Get();

		virtual void Render(float ts) {}

		void Run();
		void Close();

		float GetTime();
		RGFW_window* GetWindowHandle() const { return m_WindowHandle; }

	private:
		void Init();
		void Shutdown();

	private:
		RGFW_window* m_WindowHandle = nullptr;
		bool m_Running = false;

		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;
	};

	// Defined in client/app
	Application* CreateApplication();

} // namespace Flibbert
