#include "Demos/DemoCamera3D.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Demo
{
	DemoCamera3D::DemoCamera3D()
	    : m_Renderer(Flibbert::Renderer::Get()), m_TranslationA(-10, 5, 0),
	      m_TranslationB(0, 0, 0)
	{
		m_CameraModePerspective = std::make_shared<Flibbert::CameraModePerspective>();
		m_CameraModePerspective->VerticalFOV = 45.0f;
		m_CameraModePerspective->NearClip = 0.1f;
		m_CameraModePerspective->FarClip = 100.0f;
		m_Camera = std::make_unique<Flibbert::Camera>(m_CameraModePerspective,
		                                              glm::vec3(0.0f, 0.0f, 6.0f));

		m_CameraModeOrthographic = std::make_shared<Flibbert::CameraModeOrthographic>();
		m_CameraModeOrthographic->Size = 10.0f;
		m_CameraModeOrthographic->NearClip = -1.0f;
		m_CameraModeOrthographic->FarClip = 10.0f;

		// clang-format off
		float vertices[] = {
			-5.0f, -3.75f, 0.0f, 0.0f, 0.0f,
			5.0f,  -3.75f, 0.0f, 1.0f, 0.0f,
			5.0f,  3.75f,  0.0f, 1.0f, 1.0f,
			-5.0f, 3.75f,  0.0f, 0.0f, 1.0f,
		};

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		// clang-format on

		m_VAO = Flibbert::VertexArray::Create();
		m_VertexBuffer = Flibbert::VertexBuffer::Create(vertices, sizeof(vertices));
		Flibbert::BufferLayout layout = {
		    {Flibbert::ShaderDataType::Float3, "a_Position"},
		    {Flibbert::ShaderDataType::Float2, "a_TexCoord"},
		};
		m_VertexBuffer->SetLayout(layout);
		m_VAO->AddBuffer(*m_VertexBuffer);
		m_IndexBuffer =
		    Flibbert::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_Shader = Flibbert::Shader::Create("assets/shaders/Basic.vert",
		                                    "assets/shaders/Basic.frag");
		m_Shader->Bind();

		m_Texture = Flibbert::Texture::Create("assets/textures/neko.png");
		m_Shader->SetUniform1i("u_Texture", 0);
		m_Shader->BindUniformBlock("Matrices", 0);

		m_CameraBuffer = Flibbert::UniformBuffer::Create(sizeof(Flibbert::CameraBuffer), 0);
	}

	void DemoCamera3D::OnUpdate(float ts)
	{
		m_Camera->OnUpdate(ts);
	}

	void DemoCamera3D::OnRender()
	{
		m_Texture->Bind(0);

		Flibbert::CameraBuffer buffer{m_Camera->GetProjectionMatrix(),
		                              m_Camera->GetViewMatrix()};
		m_CameraBuffer->SetData(&buffer, sizeof(Flibbert::CameraBuffer));

		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_TranslationA);
			m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader, transform);
		}

		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_TranslationB);
			m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader, transform);
		}
	}

	void DemoCamera3D::OnImGuiRender()
	{
		if (ImGui::Checkbox("Orthographic Mode", &m_UsingOrthographicMode)) {
			if (m_UsingOrthographicMode) {
				m_Camera->SetCameraMode(m_CameraModeOrthographic);
			} else {
				m_Camera->SetCameraMode(m_CameraModePerspective);
			}
		}
		ImGui::SliderFloat3("Translation A", glm::value_ptr(m_TranslationA), -10.0f, 10.0f);
		ImGui::SliderFloat3("Translation B", glm::value_ptr(m_TranslationB), -10.0f, 10.0f);
	}

	void DemoCamera3D::OnInput(const std::shared_ptr<Flibbert::InputEvent>& event)
	{
		m_Camera->OnInput(event);
	}
} // namespace Demo
