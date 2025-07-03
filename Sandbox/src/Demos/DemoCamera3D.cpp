#include "Demos/DemoCamera3D.h"

#include "AssetPathsMacros.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Demo
{
	DemoCamera3D::DemoCamera3D()
	    : m_Renderer(Flibbert::Renderer::Get()), m_TranslationA(-10, 5, 0),
	      m_TranslationB(0, 0, 0)
	{
		m_Camera = std::make_unique<Flibbert::Camera3D>(65.0f, -1.0f, 1.0f);

		// clang-format off
		float vertices[] = {
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

		m_VAO = Flibbert::VertexArray::Create();
		m_VertexBuffer = Flibbert::VertexBuffer::Create(vertices, sizeof(vertices));
		Flibbert::BufferLayout layout = {
		    {Flibbert::ShaderDataType::Float2, "a_Position"},
		    {Flibbert::ShaderDataType::Float2, "a_TexCoord"},
		};
		m_VertexBuffer->SetLayout(layout);
		m_VAO->AddBuffer(*m_VertexBuffer);
		m_IndexBuffer =
		    Flibbert::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_Shader =
		    Flibbert::Shader::Create(SHADER_DIR "/Basic.vert", SHADER_DIR "/Basic.frag");
		m_Shader->Bind();

		m_Texture = Flibbert::Texture::Create(TEXTURE_DIR "/neko.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	void DemoCamera3D::OnUpdate(float deltaTime)
	{
		m_Camera->OnUpdate(deltaTime);
	}

	void DemoCamera3D::OnRender()
	{
		m_Texture->Bind(0);

		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_TranslationA);
			m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader,
			                m_Camera->GetProjection() * m_Camera->GetView(), transform);
		}

		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_TranslationB);
			m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader,
			                m_Camera->GetProjection() * m_Camera->GetView(), transform);
		}
	}

	void DemoCamera3D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", glm::value_ptr(m_TranslationA), -10.0f, 10.0f);
		ImGui::SliderFloat3("Translation B", glm::value_ptr(m_TranslationB), -10.0f, 10.0f);
	}
} // namespace Demo
