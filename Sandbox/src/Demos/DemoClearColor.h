#pragma once

#include "Demos/Demo.h"

namespace Demo
{
	class DemoClearColor : public Demo
	{
	public:
		DemoClearColor();

		void OnUpdate(float ts) override {}
		void OnRender() override;
		void OnImGuiRender() override;

		const char* GetName() const override { return Name; }

	public:
		static constexpr auto Name = "Clear Color";

	private:
		Flibbert::Renderer& m_Renderer;

		glm::vec4 m_ClearColor;
	};
} // namespace Demo
