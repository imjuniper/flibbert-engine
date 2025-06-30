#pragma once

struct RGFW_window;

namespace Flibbert
{
	class Renderer;
	class Window;

	class Application
	{
	public:
		Application();
		virtual ~Application();

		static Application& Get();

		virtual void Render(float ts) = 0;

		void Run();

		[[nodiscard]] float GetTime() const;
		[[nodiscard]] Window* GetWindow() const { return m_Window; }
		[[nodiscard]] Renderer* GetRenderer() const { return m_Renderer; }

	private:
		Window* m_Window = nullptr;
		Renderer* m_Renderer = nullptr;
		bool m_Running = false;

		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	// Defined in client/app
	Application* CreateApplication();
} // namespace Flibbert
