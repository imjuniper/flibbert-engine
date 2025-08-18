#include "Demos/Demo.h"
#include "Demos/DemoCamera3D.h"
#include "Demos/DemoClearColor.h"
#include "Demos/DemoFloppyBirb.h"
#include "Demos/DemoMeshGeneration.h"
#include "Demos/DemoTexture2D.h"

#include <Flibbert.h>
#include <Flibbert/Core/EntryPoint.h>

#include <imgui.h>

class Sandbox : public Flibbert::Application
{
private:
	Demo::Demo* m_currentDemo;
	Demo::DemoMenu* m_demoMenu;

public:
	explicit Sandbox(const Flibbert::ApplicationInfo& info) : Application(info)
	{
		FBT_PROFILE_FUNCTION();

		m_currentDemo = nullptr;
		m_demoMenu = new Demo::DemoMenu(m_currentDemo);
		m_currentDemo = m_demoMenu;

		m_demoMenu->RegisterDemo<Demo::DemoClearColor>("Clear Color");
		m_demoMenu->RegisterDemo<Demo::DemoTexture2D>("2D Texture");
		m_demoMenu->RegisterDemo<Demo::DemoFloppyBirb>("Floppy Birb");
		m_demoMenu->RegisterDemo<Demo::DemoCamera3D>("3D Camera");
		m_demoMenu->RegisterDemo<Demo::DemoMeshGeneration>("Mesh Generation");
	}

	~Sandbox() override
	{
		FBT_PROFILE_FUNCTION();

		if (m_currentDemo != m_demoMenu) {
			delete m_demoMenu;
		}
		delete m_currentDemo;
	}

	void OnUpdate(const float ts) override
	{
		FBT_PROFILE_FUNCTION();

		if (m_currentDemo) {
			m_currentDemo->OnUpdate(ts);
			m_currentDemo->OnRender();
		}
	}

	void OnImguiRender() override
	{
		FBT_PROFILE_FUNCTION();

		if (m_currentDemo) {
			ImGui::Begin("Sandbox");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
				    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			if (m_currentDemo != m_demoMenu && ImGui::Button("<--")) {
				delete m_currentDemo;
				m_currentDemo = m_demoMenu;
			}
			m_currentDemo->OnImGuiRender();
			ImGui::End();
		}
	}

	void OnInput(const std::shared_ptr<Flibbert::InputEvent>& event) override
	{
		FBT_PROFILE_FUNCTION();

		if (m_currentDemo) {
			m_currentDemo->OnInput(event);
		}
	}
};

Flibbert::Application* Flibbert::CreateApplication(LaunchArguments arguments)
{
	ApplicationInfo info;
	info.Name = "Sandbox";
	info.LaunchArguments = arguments;
	return new Sandbox(info);
}
