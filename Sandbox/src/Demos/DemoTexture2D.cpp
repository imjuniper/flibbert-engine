#include "Demos/DemoTexture2D.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Demo {

DemoTexture2D::DemoTexture2D()
    : m_Renderer(Flibbert::Renderer::Get()), m_TranslationA(100, 100, 0), m_TranslationB(300, 100, 0)
{
	ZoneScoped;

	auto cameraMode = std::make_shared<Flibbert::CameraModeOrthographic>();
	cameraMode->Size = 540.0f;
	cameraMode->NearClip = -1.0f;
	cameraMode->FarClip = 1.0f;
	m_Camera = std::make_unique<Flibbert::Camera>(cameraMode);

	// clang-format off
		float vertices[] = {
		     // Positions          // UVs
		    -100.0f, -75.0f, 0.0f, 0.0f, 0.0f,
		     100.0f, -75.0f, 0.0f, 1.0f, 0.0f,
		     100.0f,  75.0f, 0.0f, 1.0f, 1.0f,
		    -100.0f,  75.0f, 0.0f, 0.0f, 1.0f,
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

void DemoTexture2D::OnUpdate(float ts)
{
	ZoneScoped;

	m_Camera->OnUpdate(ts);
}

void DemoTexture2D::OnRender()
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

void DemoTexture2D::OnImGuiRender()
{
	ZoneScoped;

	ImGui::SliderFloat3("Translation A", glm::value_ptr(m_TranslationA), 0.0f, 960.0f);
	ImGui::SliderFloat3("Translation B", glm::value_ptr(m_TranslationB), 0.0f, 960.0f);
}

void DemoTexture2D::OnInput(const std::shared_ptr<Flibbert::InputEvent>& event)
{
	ZoneScoped;

	m_Camera->OnInput(event);
}

} // namespace Demo
