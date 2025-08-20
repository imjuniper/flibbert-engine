#include "Demos/Demo.h"

#include <imgui.h>

namespace Demo
{
	void DemoMenu::OnUpdate(float ts)
	{
		m_Renderer.SetClearColor(glm::vec4(0.f));
	}

	void DemoMenu::OnImGuiRender()
	{
		ZoneScoped;

		for (auto& demo : m_Demos) {
			if (ImGui::Button(demo.first)) {
				m_CurrentDemo = demo.second();
			}
		}
	}
} // namespace Demo
