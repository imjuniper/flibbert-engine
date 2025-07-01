#pragma once

#include "Demos/Demo.h"

#include <Flibbert.h>

namespace Demo
{
	class DemoTexture2D : public Demo
	{
	private:
		Flibbert::RendererBackend* m_Renderer;
		Flibbert::VertexArray* m_VAO;
		Flibbert::VertexBuffer* m_VertexBuffer;
		Flibbert::IndexBuffer* m_IndexBuffer;
		Flibbert::Shader* m_Shader;
		Flibbert::Texture* m_Texture;

		glm::mat4 m_Projection, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;

	public:
		DemoTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
} // namespace Demo
