#pragma once

#include "Demos/Demo.h"

#include <Flibbert.h>

#include <memory>

namespace Demo {

class Entity
{
public:
	virtual ~Entity() = default;
	virtual void OnUpdate(float ts) {}
	virtual void OnInput(const std::shared_ptr<Flibbert::InputEvent>& event) {}

public:
	std::shared_ptr<Flibbert::IVertexArray> m_VAO;
	std::shared_ptr<Flibbert::IVertexBuffer> m_VertexBuffer;
	std::shared_ptr<Flibbert::IIndexBuffer> m_IndexBuffer;
	std::shared_ptr<Flibbert::IShader> m_Shader;
	std::shared_ptr<Flibbert::ITexture> m_Texture;

	glm::vec2 m_Position;
	glm::vec2 m_Size;
};

class Birb : public Entity
{
public:
	Birb();

	void OnUpdate(float ts) override;
	void OnInput(const std::shared_ptr<Flibbert::InputEvent>& event) override;

private:
	float m_CurrentYSpeed = 0;
	float m_MaxFallSpeed = -400;
	float m_FallAccel = -400;
	float m_FlopSpeed = 200;

	bool m_SpacePressed = false;
};

class Pipe : public Entity
{
public:
	Pipe();
};

class DemoFloppyBirb : public Demo
{
public:
	DemoFloppyBirb();

	void OnUpdate(float ts) override;
	void OnRender() override;
	void OnImGuiRender() override;
	void OnInput(const std::shared_ptr<Flibbert::InputEvent>& event) override;

	const char* GetName() const override
	{
		return Name;
	}

public:
	static constexpr auto Name = "Floppy Birb";

private:
	Flibbert::Renderer& m_Renderer;
	std::shared_ptr<Flibbert::IUniformBuffer> m_PerFrameBuffer;
	std::shared_ptr<Flibbert::IUniformBuffer> m_PerObjectBuffer;

	std::unique_ptr<Flibbert::Camera> m_Camera;

	Birb m_Birb;
	Pipe m_Pipe;
};

} // namespace Demo
