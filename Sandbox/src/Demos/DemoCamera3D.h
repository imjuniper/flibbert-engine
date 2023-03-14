#pragma once

#include "Demos/Demo.h"
#include <Flibbert.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace Demo {

	class DemoCamera3D : public Demo {
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		Camera* m_Camera;
		glm::vec3 m_TranslationA, m_TranslationB;

	public:
		DemoCamera3D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};

} // namespace Demo
