#pragma once

#include "Demo.h"

namespace Demo
{

	class DemoClearColor : public Demo
	{
	private:
		float m_ClearColor[4];

	public:
		DemoClearColor();

		void OnRender() override;
		void OnImGuiRender() override;
	};

} // namespace Demo
