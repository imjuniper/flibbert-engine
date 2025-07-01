#pragma once

#include "Demos/Demo.h"

#include <Flibbert.h>

namespace Demo
{
	class DemoCamera3D : public Demo
	{
	private:
		Flibbert::VertexArray* m_VAO;
		Flibbert::VertexBuffer* m_VertexBuffer;
		Flibbert::IndexBuffer* m_IndexBuffer;
		Flibbert::Shader* m_Shader;
		Flibbert::Texture* m_Texture;

		Camera* m_Camera;
		glm::vec3 m_TranslationA, m_TranslationB;

	public:
		DemoCamera3D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
} // namespace Demo
