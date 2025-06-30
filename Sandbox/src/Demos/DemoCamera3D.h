#pragma once

#include "Demos/Demo.h"

#include <Flibbert.h>

#include <memory>

namespace Demo
{
	class DemoCamera3D : public Demo
	{
	private:
		std::unique_ptr<Flibbert::OpenGLVertexArray> m_VAO;
		std::unique_ptr<Flibbert::OpenGLVertexBuffer> m_VertexBuffer;
		std::unique_ptr<Flibbert::OpenGLIndexBuffer> m_IndexBuffer;
		std::unique_ptr<Flibbert::OpenGLShader> m_Shader;
		std::unique_ptr<Flibbert::OpenGLTexture> m_Texture;

		Camera* m_Camera;
		glm::vec3 m_TranslationA, m_TranslationB;

	public:
		DemoCamera3D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
} // namespace Demo
