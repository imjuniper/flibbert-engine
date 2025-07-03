#pragma once

#include <memory>
#include <string>

struct RGFW_window;

namespace Flibbert
{
	class Renderer;
	class Window;

	struct LaunchArguments
	{
		int Count;
		char** Arguments;

		const char* operator[](const int index) const
		{
			//assert(index < Count);
			return Arguments[index];
		}
	};

	struct ApplicationInfo
	{
		std::string Name;
		LaunchArguments LaunchArguments;
	};

	class Application
	{
	public:
		explicit Application(const ApplicationInfo& info);
		virtual ~Application();

		static Application& Get();

		virtual void Render(float ts) = 0;

		void Run();

		[[nodiscard]] float GetTime() const;
		[[nodiscard]] Window& GetWindow() const;
		[[nodiscard]] Renderer& GetRenderer() const;

	private:
		std::unique_ptr<Window> m_Window = nullptr;
		std::unique_ptr<Renderer> m_Renderer = nullptr;
		bool m_Running = false;

		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	// Defined in client/app
	Application* CreateApplication(LaunchArguments arguments);
} // namespace Flibbert
