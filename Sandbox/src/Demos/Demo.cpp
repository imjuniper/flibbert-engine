#include "Demos/Demo.h"

#include <imgui.h>

namespace Demo
{
	void DemoMenu::OnImGuiRender()
	{
		for (auto& demo : m_Demos) {
			if (ImGui::Button(demo.first)) {
				m_CurrentDemo = demo.second();
			}
		}
	}
} // namespace Demo
