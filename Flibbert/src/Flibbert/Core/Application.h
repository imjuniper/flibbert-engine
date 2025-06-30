#pragma once

struct RGFW_window;

namespace Flibbert
{
	class Window;

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

		[[nodiscard]] Window* GetWindow() const { return m_Window; }

	private:
		void Init();
		void Shutdown();

	private:
		Window* m_Window = nullptr;
		bool m_Running = false;

		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;
	};

	// Defined in client/app
	Application* CreateApplication();

} // namespace Flibbert
