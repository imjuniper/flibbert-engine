#pragma once

#include "Demos/Demo.h"
#include <Flibbert.h>

#include <memory>

namespace Demo
{
	class Entity
	{
	public:
		std::unique_ptr<Flibbert::OpenGLVertexArray> m_VAO;
		std::unique_ptr<Flibbert::OpenGLVertexBuffer> m_VertexBuffer;
		std::unique_ptr<Flibbert::OpenGLIndexBuffer> m_IndexBuffer;
		std::unique_ptr<Flibbert::OpenGLShader> m_Shader;
		std::unique_ptr<Flibbert::OpenGLTexture> m_Texture;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

	public:
		virtual void OnUpdate(float deltaTime) {};
	};

	class Birb : public Entity
	{
	private:
		float m_CurrentYSpeed = 0;
		float m_MaxFallSpeed = -400;
		float m_FallAccel = -400;
		float m_FlopSpeed = 200;

		bool m_SpacePressed = false;

	public:
		Birb();

		void OnUpdate(float deltaTime) override;
	};

	class Pipe : public Entity
	{
	private:

	public:
		Pipe();
	};

	class DemoFloppyBirb : public Demo
	{
	private:
		glm::mat4 m_Projection, m_View;

		Birb m_Birb;
		Pipe m_Pipe;

	public:
		DemoFloppyBirb();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
} // namespace Demo
