#include "Demos/DemoClearColor.h"

#include <Flibbert.h>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Demo
{
	DemoClearColor::DemoClearColor()
	    : m_Renderer(Flibbert::Renderer::Get()), m_ClearColor{1.0f, 0.0f, 0.0f, 0.0f},
	      m_InitialColor(m_Renderer.GetClearColor())
	{
	}

	DemoClearColor::~DemoClearColor()
	{
		m_Renderer.SetClearColor(m_InitialColor);
	}

	void DemoClearColor::OnRender()
	{
		m_Renderer.SetClearColor(m_ClearColor);
	}

	void DemoClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
	}
} // namespace Demo
