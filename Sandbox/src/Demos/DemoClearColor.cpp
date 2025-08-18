#include "Demos/DemoClearColor.h"

#include "Flibbert/Debug/Instrumentor.h"

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
		FBT_PROFILE_FUNCTION();

		m_Renderer.SetClearColor(m_InitialColor);
	}

	void DemoClearColor::OnRender()
	{
		FBT_PROFILE_FUNCTION();

		m_Renderer.SetClearColor(m_ClearColor);
	}

	void DemoClearColor::OnImGuiRender()
	{
		FBT_PROFILE_FUNCTION();

		ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
	}
} // namespace Demo
