#pragma once

#include "Demos/Demo.h"

namespace Demo
{
	class DemoClearColor : public Demo
	{
	public:
		DemoClearColor();
		~DemoClearColor() override;

		void OnUpdate(float deltaTime) override {}
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Flibbert::Renderer& m_Renderer;

		glm::vec4 m_ClearColor;
		glm::vec4 m_InitialColor;
	};
} // namespace Demo
