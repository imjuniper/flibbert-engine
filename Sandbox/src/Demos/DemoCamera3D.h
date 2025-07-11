#pragma once

#include "Demos/Demo.h"

#include <Flibbert.h>

namespace Demo
{
	class DemoCamera3D : public Demo
	{
	public:
		DemoCamera3D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Flibbert::Renderer& m_Renderer;

		std::shared_ptr<Flibbert::VertexArray> m_VAO;
		std::shared_ptr<Flibbert::VertexBuffer> m_VertexBuffer;
		std::shared_ptr<Flibbert::IndexBuffer> m_IndexBuffer;
		std::shared_ptr<Flibbert::Shader> m_Shader;
		std::shared_ptr<Flibbert::Texture> m_Texture;
		std::shared_ptr<Flibbert::UniformBuffer> m_CameraBuffer;

		std::unique_ptr<Flibbert::Camera> m_Camera;

		glm::vec3 m_TranslationA, m_TranslationB;
	};
} // namespace Demo
