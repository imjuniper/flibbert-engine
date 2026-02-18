#include "Demos/DemoCamera3D.h"
#include "Demos/Demo.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Demo {

DemoCamera3D::DemoCamera3D() : m_Renderer(Flibbert::Renderer::Get()), m_TranslationA(-10, 5, 0), m_TranslationB(0, 0, 0)
{
	ZoneScoped;

	m_CameraModePerspective = std::make_shared<Flibbert::CameraModePerspective>();
	m_CameraModePerspective->VerticalFOV = 45.0f;
	m_CameraModePerspective->NearClip = 0.1f;
	m_CameraModePerspective->FarClip = 100.0f;
	m_Camera = std::make_unique<Flibbert::Camera>(m_CameraModePerspective, glm::vec3(0.0f, 0.0f, 6.0f));

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

	m_VertexBuffer = Flibbert::IVertexBuffer::Create(vertices, sizeof(vertices));
	Flibbert::BufferLayout layout = {
	    {Flibbert::ShaderDataType::Float3, "a_Position"},
	    {Flibbert::ShaderDataType::Float2, "a_TexCoord"},
	};
	m_VertexBuffer->SetLayout(layout);

	m_IndexBuffer = Flibbert::IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

	m_VAO = Flibbert::IVertexArray::Create();
	m_VAO->AddVertexBuffer(m_VertexBuffer);
	m_VAO->SetIndexBuffer(m_IndexBuffer);

	m_Shader = Flibbert::IShader::Create("assets/shaders/Basic.vert", "assets/shaders/Basic.frag");
	m_Shader->BindUniformBuffer("PerFrameData", 0);
	m_Shader->BindUniformBuffer("PerObjectData", 1);
	m_Shader->BindUniformBuffer("BasicData", 2);

	m_PerFrameBuffer = Flibbert::IUniformBuffer::Create(sizeof(PerFrameUniformData), 0);
	m_PerObjectBuffer = Flibbert::IUniformBuffer::Create(sizeof(PerObjectUniformData), 1);
	m_BasicDataBuffer = Flibbert::IUniformBuffer::Create(sizeof(BasicUniformData), 2);

	m_Texture = Flibbert::ITexture::Create("assets/textures/neko.png");
	const BasicUniformData basicData{m_Texture->GetHandle()};
	m_BasicDataBuffer->SetData(&basicData, sizeof(BasicUniformData));
}

void DemoCamera3D::OnUpdate(float ts)
{
	ZoneScoped;

	m_Camera->OnUpdate(ts);
}

void DemoCamera3D::OnRender()
{
	ZoneScoped;

	const PerFrameUniformData perFrameBuffer{m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix(),
	                                         m_Camera->GetPosition()};
	m_PerFrameBuffer->SetData(&perFrameBuffer, sizeof(PerFrameUniformData));

	m_Texture->MakeResident();

	{
		const PerObjectUniformData buffer{glm::translate(glm::mat4(1.0f), m_TranslationA)};
		m_PerObjectBuffer->SetData(&buffer, sizeof(PerObjectUniformData));
		m_Renderer.Submit(m_VAO, m_Shader);
	}

	{
		const PerObjectUniformData buffer{glm::translate(glm::mat4(1.0f), m_TranslationB)};
		m_PerObjectBuffer->SetData(&buffer, sizeof(PerObjectUniformData));
		m_Renderer.Submit(m_VAO, m_Shader);
	}

	m_Texture->MakeNonResident();
}

void DemoCamera3D::OnImGuiRender()
{
	ZoneScoped;

	if (ImGui::Checkbox("Orthographic Mode", &m_UsingOrthographicMode)) {
		if (m_UsingOrthographicMode) {
			m_Camera->SetCameraMode(m_CameraModeOrthographic);
		}
		else {
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
