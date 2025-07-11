#pragma once

#include "Demos/Demo.h"

#include <Flibbert.h>

#include <memory>

namespace Demo
{
	class Entity
	{
	public:
		virtual ~Entity() = default;
		virtual void OnUpdate(float deltaTime) {}

	public:
		std::shared_ptr<Flibbert::VertexArray> m_VAO;
		std::shared_ptr<Flibbert::VertexBuffer> m_VertexBuffer;
		std::shared_ptr<Flibbert::IndexBuffer> m_IndexBuffer;
		std::shared_ptr<Flibbert::Shader> m_Shader;
		std::shared_ptr<Flibbert::Texture> m_Texture;

		glm::vec2 m_Position;
		glm::vec2 m_Size;
	};

	class Birb : public Entity
	{
	public:
		Birb();

		void OnUpdate(float deltaTime) override;
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

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Flibbert::Renderer& m_Renderer;
		std::shared_ptr<Flibbert::UniformBuffer> m_CameraBuffer;

		std::unique_ptr<Flibbert::Camera> m_Camera;

		Birb m_Birb;
		Pipe m_Pipe;

	};
} // namespace Demo
