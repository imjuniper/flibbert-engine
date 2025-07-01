#include "Demos/DemoClearColor.h"

#include <Flibbert.h>

#include <imgui.h>

namespace Demo
{
	DemoClearColor::DemoClearColor() : m_ClearColor{1.0f, 0.0f, 0.0f, 0.0f}, m_InitialColor()
	{
		m_Renderer = Flibbert::Application::Get().GetRenderer()->GetBackend();
		m_InitialColor = m_Renderer->GetClearColor();
	}

	DemoClearColor::~DemoClearColor()
	{
		m_Renderer->SetClearColor(m_InitialColor);
	}

	void DemoClearColor::OnRender()
	{
		m_Renderer->SetClearColor(
		    {m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]});
	}

	void DemoClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}
} // namespace Demo
