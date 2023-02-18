#include "DemoClearColor.h"

#include "opengl/Renderer.h"

namespace Demo
{

	DemoClearColor::DemoClearColor() : m_ClearColor{1.0f, 0.0f, 0.0f, 0.0f} {}

	void DemoClearColor::OnRender()
	{
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2],
				    m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void DemoClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}

} // namespace Demo
