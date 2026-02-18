#pragma once

#include "Flibbert/Core/ApplicationSubsystem.h"
#include "Flibbert/Core/Base.h"
#include "Flibbert/Renderer/Renderer.h"
#include "Platform/Desktop/Window.h"

#include <memory>
#include <string>

namespace Flibbert {

struct InputEvent;

struct LaunchArguments
{
	int Count;
	char** Arguments;

	const char* operator[](const int index) const
	{
		FBT_CORE_ENSURE(index < Count);
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

	void InitializeSubsystems();
	void ShutdownSubsystems();

	template <typename T>
	std::shared_ptr<T> GetSubsystem()
	{
		for (auto subsystem : m_Subsystems) {
			if (auto asType = std::static_pointer_cast<T>(subsystem)) {
				return asType;
			}
		}

		return nullptr;
	}

	void Run();
	void Close();

	void DispatchInputEvent(const std::shared_ptr<InputEvent>& event);

	[[nodiscard]] Window& GetWindow() const;
	[[nodiscard]] Renderer& GetRenderer() const;

private:
	virtual void OnUpdate(double ts) = 0;
	virtual void OnRender() = 0;
	virtual void OnInput(const std::shared_ptr<InputEvent>& event) = 0;

	void HandleWindowClosed(Window& window);

private:
	std::unique_ptr<Window> m_Window = nullptr;
	std::unique_ptr<Renderer> m_Renderer = nullptr;
	bool m_Running = false;

	double m_FrameTime = 0.0;
	double m_LastFrameTime = 0.0;

	std::vector<std::shared_ptr<ApplicationSubsystem>> m_Subsystems;

private:
	static Application* s_Instance;
};

// Defined in client/app
Application* CreateApplication(LaunchArguments arguments);

} // namespace Flibbert
