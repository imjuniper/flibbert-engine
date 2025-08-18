#include "Demos/Demo.h"

#include "Flibbert/Debug/Instrumentor.h"

#include <imgui.h>

namespace Demo
{
	void DemoMenu::OnImGuiRender()
	{
		FBT_PROFILE_FUNCTION();

		for (auto& demo : m_Demos) {
			if (ImGui::Button(demo.first)) {
				m_CurrentDemo = demo.second();
			}
		}
	}
} // namespace Demo
