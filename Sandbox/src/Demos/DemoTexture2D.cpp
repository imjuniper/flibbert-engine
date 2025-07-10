#include "Demos/DemoTexture2D.h"

#include "AssetPathsMacros.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Demo
{
	DemoTexture2D::DemoTexture2D()
	    : m_Renderer(Flibbert::Renderer::Get()),
	      m_Projection(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
	      m_TranslationA(100, 100, 0), m_TranslationB(300, 100, 0)
	{
		// clang-format off
		float vertices[] = {
		    -100.0f, -75.0f, 0.0f, 0.0f,
		    100.0f,  -75.0f, 1.0f, 0.0f,
		    100.0f,  75.0f,  1.0f, 1.0f,
		    -100.0f, 75.0f,  0.0f, 1.0f,
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
		m_Shader->BindUniformBlock("Matrices", 0);

		m_CameraBuffer = Flibbert::UniformBuffer::Create(sizeof(Flibbert::CameraBuffer), 0);

		Flibbert::CameraBuffer buffer{m_Projection, m_View};
		m_CameraBuffer->SetData(&buffer, sizeof(Flibbert::CameraBuffer));
	}

	void DemoTexture2D::OnUpdate(float deltaTime) {}

	void DemoTexture2D::OnRender()
	{
		m_Texture->Bind(0);

		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_TranslationA);
			m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader, transform);
		}

		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_TranslationB);
			m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader, transform);
		}
	}

	void DemoTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", glm::value_ptr(m_TranslationA), 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", glm::value_ptr(m_TranslationB), 0.0f, 960.0f);
	}
} // namespace Demo
