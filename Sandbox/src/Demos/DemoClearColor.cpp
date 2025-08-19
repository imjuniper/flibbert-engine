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
		ZoneScoped;

		m_Renderer.SetClearColor(m_InitialColor);
	}

	void DemoClearColor::OnRender()
	{
		ZoneScoped;

		m_Renderer.SetClearColor(m_ClearColor);
	}

	void DemoClearColor::OnImGuiRender()
	{
		ZoneScoped;

		ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
	}
} // namespace Demo
