#pragma once

#include "Demos/Demo.h"

#include <Flibbert.h>

namespace Demo
{
	class DemoTexture2D : public Demo
	{
	private:
		Flibbert::Renderer& m_Renderer;

		std::shared_ptr<Flibbert::VertexArray> m_VAO;
		std::shared_ptr<Flibbert::VertexBuffer> m_VertexBuffer;
		std::shared_ptr<Flibbert::IndexBuffer> m_IndexBuffer;
		std::shared_ptr<Flibbert::Shader> m_Shader;
		std::shared_ptr<Flibbert::Texture> m_Texture;

		glm::mat4 m_Projection, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;

	public:
		DemoTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
} // namespace Demo
