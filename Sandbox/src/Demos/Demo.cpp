#include "Demos/Demo.h"

namespace Demo
{
	DemoMenu::DemoMenu(Demo*& currentDemoPointer) : m_CurrentDemo(currentDemoPointer) {}

	void DemoMenu::OnImGuiRender()
	{
		for (auto& demo : m_Demos) {
			if (ImGui::Button(demo.first.c_str())) {
				m_CurrentDemo = demo.second();
			}
		}
	}
} // namespace Demo
