#include "Demos/DemoTexture2D.h"

#include "Platform/D3D11/D3D11Buffer.h"
#include "Platform/D3D11/D3D11Shader.h"

#include <d3d11.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Demo
{
	DemoTexture2D::DemoTexture2D()
	    : m_Renderer(Flibbert::Renderer::Get()), m_TranslationA(100, 100, 0),
	      m_TranslationB(300, 100, 0)
	{
		auto cameraMode = std::make_shared<Flibbert::CameraModeOrthographic>();
		cameraMode->Size = 540.0f;
		cameraMode->NearClip = -1.0f;
		cameraMode->FarClip = 1.0f;
		m_Camera = std::make_unique<Flibbert::Camera>(cameraMode);

		// clang-format off
		float vertices[] = {
		    -100.0f, -75.0f, 0.0f, 0.0f, 0.0f,
		     100.0f, -75.0f, 0.0f, 1.0f, 0.0f,
		     100.0f,  75.0f, 0.0f, 1.0f, 1.0f,
		    -100.0f,  75.0f, 0.0f, 0.0f, 1.0f,
		};

		uint32_t indices[] = {
		    0, 2, 1,
		    2, 0, 3
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
		m_Shader->BindUniformBuffer("Matrices", 0);

		constexpr D3D11_INPUT_ELEMENT_DESC vertexInputLayoutInfo[] = {
		    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,
		     0}};

		auto d3dshader = std::dynamic_pointer_cast<Flibbert::D3D11Shader>(m_Shader);

		const auto device =
		    static_cast<ID3D11Device*>(Flibbert::Renderer::Get().GetDevice());
		device->CreateInputLayout(vertexInputLayoutInfo, _countof(vertexInputLayoutInfo),
		                          d3dshader->m_VertexShaderBlob->GetBufferPointer(),
		                          d3dshader->m_VertexShaderBlob->GetBufferSize(),
		                          &m_VertexLayout);

		m_MatricesBuffer = Flibbert::UniformBuffer::Create(sizeof(MatricesBuffer), 0);
	}

	void DemoTexture2D::OnUpdate(float ts)
	{
		m_Camera->OnUpdate(ts);
	}

	void DemoTexture2D::OnRender()
	{
		m_Texture->Bind(0);

		const auto context =
		    static_cast<ID3D11DeviceContext*>(Flibbert::Renderer::Get().GetDeviceContext());
		auto constbuff =
		    std::dynamic_pointer_cast<Flibbert::D3D11UniformBuffer>(m_MatricesBuffer)
			->m_ConstantBuffer;

		MatricesBuffer buffer{m_Camera->GetProjectionMatrix(), m_Camera->GetViewMatrix()};

		{
			context->IASetInputLayout(m_VertexLayout);
			context->VSSetConstantBuffers(0, 1, &constbuff);
			buffer.Model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			m_MatricesBuffer->SetData(&buffer, sizeof(MatricesBuffer));
			m_Renderer.Draw(m_VAO, m_Shader);
		}

		{
			context->IASetInputLayout(m_VertexLayout);
			context->VSSetConstantBuffers(0, 1, &constbuff);
			buffer.Model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			m_MatricesBuffer->SetData(&buffer, sizeof(MatricesBuffer));
			m_Renderer.Draw(m_VAO, m_Shader);
		}
	}

	void DemoTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", glm::value_ptr(m_TranslationA), 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", glm::value_ptr(m_TranslationB), 0.0f, 960.0f);
	}

	void DemoTexture2D::OnInput(const std::shared_ptr<Flibbert::InputEvent>& event)
	{
		m_Camera->OnInput(event);
	}
} // namespace Demo
