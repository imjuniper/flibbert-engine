#pragma once

#include "Demos/Demo.h"

namespace Demo
{
	class DemoClearColor : public Demo
	{
	private:
		float m_ClearColor[4];
		glm::vec4 m_InitialColor;

		Flibbert::RendererBackend* m_Renderer;

	public:
		DemoClearColor();
		~DemoClearColor() override;

		void OnRender() override;
		void OnImGuiRender() override;
	};
} // namespace Demo
