#pragma once

#include "Demos/Demo.h"

namespace Demo {

class DemoClearColor : public Demo
{
public:
	DemoClearColor();

	void OnUpdate(float ts) override {}
	void OnRender() override;
	void OnImGuiRender() override;

private:
	Flibbert::Renderer& m_Renderer;

	glm::vec4 m_ClearColor;
};

} // namespace Demo
