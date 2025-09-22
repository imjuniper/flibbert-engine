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
	std::unique_ptr<Demo::Demo> m_CurrentDemo = nullptr;

	std::vector<std::pair<const char*, std::function<std::unique_ptr<Demo::Demo>()>>> m_Demos;

public:
	explicit Sandbox(const Flibbert::ApplicationInfo& info) : Application(info)
	{
		ZoneScoped;

		RegisterDemo<Demo::DemoClearColor>();
		RegisterDemo<Demo::DemoTexture2D>();
		RegisterDemo<Demo::DemoFloppyBirb>();
		RegisterDemo<Demo::DemoCamera3D>();
		RegisterDemo<Demo::DemoMeshGeneration>();
	}

	template <typename TDemo>
	void RegisterDemo()
	{
		ZoneScoped;

		FBT_INFO("Registering demo {}", TDemo::Name);
		m_Demos.emplace_back(TDemo::Name, []() { return std::make_unique<TDemo>(); });
	}

	void OnUpdate(const double ts) override
	{
		ZoneScoped;

		if (m_CurrentDemo) {
			m_CurrentDemo->OnUpdate(ts);
		}
	}

	void OnRender() override
	{
		ZoneScoped;

		if (m_CurrentDemo) {
			m_CurrentDemo->OnRender();
		}
	}


	void BeginMainImguiWindow()
	{
		// Make the window fill the viewport
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		// Hide the main window borders, background, etc.
		constexpr ImGuiWindowFlags windowFlags =
		    ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
		    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar |
		    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
		    ImGuiWindowFlags_NoNavFocus;

		static bool mainWindowOpen = true;
		ImGui::Begin("Sandbox", &mainWindowOpen, windowFlags);

		ImGui::PopStyleVar(3);

		// Create the dock space if available
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			constexpr ImGuiDockNodeFlags dockspaceFlags =
			    ImGuiDockNodeFlags_PassthruCentralNode |
			    ImGuiDockNodeFlags_NoDockingOverCentralNode |
			    ImGuiDockNodeFlags_AutoHideTabBar;
			ImGui::DockSpace(ImGui::GetID("SandboxDockSpace"), ImVec2(0.0f, 0.0f),
			                 dockspaceFlags);
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Demos")) {
				for (auto& [demoName, createDemo] : m_Demos) {
					if (ImGui::MenuItem(demoName)) {
						m_CurrentDemo = createDemo();
					}
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) {
					Close();
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			const std::string stats =
			    std::format("{:.2f} FPS ({:.2f} ms)", ImGui::GetIO().Framerate,
			                1000.0f / ImGui::GetIO().Framerate);
			const auto statsSize = ImGui::CalcTextSize(stats.c_str());

			ImGui::SameLine(ImGui::GetWindowWidth() - statsSize.x - 4.0f);
			ImGui::Text(stats.c_str());

			ImGui::EndMenuBar();
		}
	}

	void EndMainImguiWindow() { ImGui::End(); }

	void OnImguiRender() override
	{
		ZoneScoped;

		BeginMainImguiWindow();

		if (m_CurrentDemo) {
			ImGui::Begin(m_CurrentDemo->GetName());
			m_CurrentDemo->OnImGuiRender();
			ImGui::End();
		}

		EndMainImguiWindow();
	}

	void OnInput(const std::shared_ptr<Flibbert::InputEvent>& event) override
	{
		ZoneScoped;

		if (m_CurrentDemo) {
			m_CurrentDemo->OnInput(event);
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
