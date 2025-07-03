#pragma once

#include "Demos/Demo.h"

namespace Demo
{
	class DemoClearColor : public Demo
	{
	private:
		Flibbert::Renderer& m_Renderer;

		glm::vec4 m_ClearColor;
		glm::vec4 m_InitialColor;

	public:
		DemoClearColor();
		~DemoClearColor() override;

		void OnRender() override;
		void OnImGuiRender() override;
	};
} // namespace Demo
