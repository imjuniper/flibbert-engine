#pragma once

#include "Flibbert.h"
#include "Flibbert/Debug/Profiling.h"

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace Demo {

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

struct BasicUniformData
{
	uint64_t Texture;
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
};

} // namespace Demo
