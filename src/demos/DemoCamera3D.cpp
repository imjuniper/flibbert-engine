#include "DemoCamera3D.h"

#include "opengl/asset.h"

namespace Demo
{

	DemoCamera3D::DemoCamera3D() : m_TranslationA(-10, 5, 0), m_TranslationB(0, 0, 0)
	{
		m_Camera = new Camera(65.0f, -1.0f, 1.0f);
		// clang-format off
		float positions[] = {
			-5.0f, -3.75f, 0.0f, 0.0f,
			5.0f,  -3.75f, 1.0f, 0.0f,
			5.0f,  3.75f,  1.0f, 1.0f,
			-5.0f, 3.75f,  0.0f, 1.0f,
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

	void DemoCamera3D::OnUpdate(float deltaTime)
	{
		m_Camera->OnUpdate(deltaTime);
	}

	void DemoCamera3D::OnRender()
	{
		Renderer renderer;

		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Camera->GetProjection() * m_Camera->GetView() * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Camera->GetProjection() * m_Camera->GetView() * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void DemoCamera3D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, -10.0f, 10.0f);
	}

} // namespace Demo
