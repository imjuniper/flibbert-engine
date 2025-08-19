#include "Demos/DemoCamera3D.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Demo
{
	DemoCamera3D::DemoCamera3D()
	    : m_Renderer(Flibbert::Renderer::Get()), m_TranslationA(-10, 5, 0),
	      m_TranslationB(0, 0, 0)
	{
		ZoneScoped;

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

		m_VertexBuffer = Flibbert::VertexBuffer::Create(vertices, sizeof(vertices));
		Flibbert::BufferLayout layout = {
		    {Flibbert::ShaderDataType::Float3, "a_Position"},
		    {Flibbert::ShaderDataType::Float2, "a_TexCoord"},
		};
		m_VertexBuffer->SetLayout(layout);

		m_IndexBuffer =
		    Flibbert::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_VAO = Flibbert::VertexArray::Create();
		m_VAO->AddVertexBuffer(m_VertexBuffer);
		m_VAO->SetIndexBuffer(m_IndexBuffer);

		m_Shader = Flibbert::Shader::Create("assets/shaders/Basic.vert",
		                                    "assets/shaders/Basic.frag");
		m_Shader->Bind();

		m_Texture = Flibbert::Texture::Create("assets/textures/neko.png");
		m_Shader->SetUniform1i("u_Texture", 0);
		m_Shader->BindUniformBuffer("PerFrameData", 0);
		m_Shader->BindUniformBuffer("PerObjectData", 1);

		m_PerFrameBuffer =
		    Flibbert::UniformBuffer::Create(sizeof(PerFrameUniformData), 0);
		m_PerObjectBuffer =
		    Flibbert::UniformBuffer::Create(sizeof(PerObjectUniformData), 1);
	}

	void DemoCamera3D::OnUpdate(float ts)
	{
		ZoneScoped;

		m_Camera->OnUpdate(ts);
	}

	void DemoCamera3D::OnRender()
	{
		ZoneScoped;

		m_Texture->Bind(0);

		const PerFrameUniformData perFrameBuffer{m_Camera->GetViewMatrix(),
		                                           m_Camera->GetProjectionMatrix(),
		                                           m_Camera->GetPosition()};
		m_PerFrameBuffer->SetData(&perFrameBuffer, sizeof(PerFrameUniformData));

		{
			const PerObjectUniformData buffer{
			    glm::translate(glm::mat4(1.0f), m_TranslationA)};
			m_PerObjectBuffer->SetData(&buffer, sizeof(PerObjectUniformData));
			m_Renderer.Draw(m_VAO, m_Shader);
		}

		{
			const PerObjectUniformData buffer{
			    glm::translate(glm::mat4(1.0f), m_TranslationB)};
			m_PerObjectBuffer->SetData(&buffer, sizeof(PerObjectUniformData));
			m_Renderer.Draw(m_VAO, m_Shader);
		}
	}

	void DemoCamera3D::OnImGuiRender()
	{
		ZoneScoped;

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
		ZoneScoped;

		m_Camera->OnInput(event);
	}
} // namespace Demo
