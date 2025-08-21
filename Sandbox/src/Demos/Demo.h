#pragma once

#include "Flibbert/Debug/Profiling.h"

#include <Flibbert.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace Demo
{
	struct PerFrameUniformData
	{
		glm::mat4 View;
		glm::mat4 Projection;
		glm::vec3 ViewPosition;
		// Lights?
	};

	struct PerObjectUniformData
	{
		glm::mat4 Transform;
		// Probably also material properties?
	};

	class Demo
	{
	public:
		Demo() = default;
		virtual ~Demo() = default;

		virtual void OnUpdate(float ts) {};
		virtual void OnRender() {};
		virtual void OnImGuiRender() {};
		virtual void OnInput(const std::shared_ptr<Flibbert::InputEvent>& event) {}

		virtual const char* GetName() const = 0;
	};
} // namespace Demo
