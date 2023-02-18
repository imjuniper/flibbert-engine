#include "DemoTexture2D.h"

#include "opengl/asset.h"

namespace Demo
{

	DemoTexture2D::DemoTexture2D()
	    : m_Projection(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
	      m_TranslationA(100, 100, 0), m_TranslationB(300, 100, 0)
	{
		// clang-format off
		float positions[] = {
		    -100.0f, -75.0f, 0.0f, 0.0f,
		    100.0f,  -75.0f, 1.0f, 0.0f,
		    100.0f,  75.0f,  1.0f, 1.0f,
		    -100.0f, 75.0f,  0.0f, 1.0f,
		};

		unsigned int indices[] = {
		    0, 1, 2,
		    2, 3, 0
		};
		// clang-format on

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader =
		    std::make_unique<Shader>(SHADER_DIR "/Basic.vert", SHADER_DIR "/Basic.frag");
		m_Shader->Bind();
		m_Texture = std::make_unique<Texture>(TEXTURE_DIR "/neko.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	void DemoTexture2D::OnUpdate(float deltaTime) {}

	void DemoTexture2D::OnRender()
	{
		Renderer renderer;

		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Projection * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Projection * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void DemoTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
	}

} // namespace Demo
