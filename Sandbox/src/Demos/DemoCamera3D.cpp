#include "Demos/DemoCamera3D.h"

#include "AssetPathsMacros.h"

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

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		// clang-format on

		m_VAO = std::make_unique<Flibbert::OpenGLVertexArray>();
		m_VertexBuffer = std::make_unique<Flibbert::OpenGLVertexBuffer>(
		    positions, 4 * 4 * sizeof(float));
		Flibbert::VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<Flibbert::OpenGLIndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Flibbert::OpenGLShader>(SHADER_DIR "/Basic.vert",
		                                                    SHADER_DIR "/Basic.frag");
		m_Shader->Bind();
		m_Texture = std::make_unique<Flibbert::OpenGLTexture>(TEXTURE_DIR "/neko.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	void DemoCamera3D::OnUpdate(float deltaTime)
	{
		m_Camera->OnUpdate(deltaTime);
	}

	void DemoCamera3D::OnRender()
	{
		Flibbert::OpenGLRendererBackend* renderer =
		    static_cast<Flibbert::OpenGLRendererBackend*>(
			Flibbert::Application::Get().GetRenderer()->GetBackend());

		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Camera->GetProjection() * m_Camera->GetView() * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer->Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Camera->GetProjection() * m_Camera->GetView() * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer->Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void DemoCamera3D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, -10.0f, 10.0f);
	}
} // namespace Demo
