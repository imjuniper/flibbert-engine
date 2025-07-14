#pragma once

#include "Flibbert/Core/Base.h"

#include <memory>
#include <string>

struct RGFW_window;

namespace Flibbert
{
	class Renderer;
	class Window;
	struct InputEvent;

	struct LaunchArguments {
		int Count;
		char** Arguments;

		const char* operator[](const int index) const
		{
			FBT_CORE_ENSURE(index < Count);
			return Arguments[index];
		}
	};

	struct ApplicationInfo {
		std::string Name;
		LaunchArguments LaunchArguments;
	};

	class Application
	{
	public:
		explicit Application(const ApplicationInfo& info);
		virtual ~Application();

		static Application& Get();

		void Run();

		[[nodiscard]] float GetTime() const;
		[[nodiscard]] Window& GetWindow() const;
		[[nodiscard]] Renderer& GetRenderer() const;

	private:
		virtual void OnUpdate(float ts) = 0;
		virtual void OnImguiRender() = 0;
		virtual void OnInput(const std::shared_ptr<InputEvent>& event) = 0;

		void HandleWindowClosed(Window& window);
		void DispatchInputEvent(const std::shared_ptr<InputEvent>& event);

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
