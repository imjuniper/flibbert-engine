#include "Demos/DemoFloppyBirb.h"

#include <Flibbert.h>

#include <imgui.h>

namespace Demo
{
#pragma region Birb
	Birb::Birb()
	{
		m_Position = glm::vec2(100, 500);
		m_Size = glm::vec2(50, 50);

		// clang-format off
		float vertices[] = {
			m_Size.x / -2.0f, m_Size.y / -2.0f,
			m_Size.x / 2.0f,  m_Size.y / -2.0f,
			m_Size.x / 2.0f,  m_Size.y / 2.0f,
			m_Size.x / -2.0f, m_Size.y / 2.0f,
		};

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		// clang-format on

		m_VertexBuffer = Flibbert::VertexBuffer::Create(vertices, sizeof(vertices));
		Flibbert::BufferLayout layout = {
		    {Flibbert::ShaderDataType::Float2, "a_Position"},
		};
		m_VertexBuffer->SetLayout(layout);

		m_IndexBuffer =
		    Flibbert::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_VAO = Flibbert::VertexArray::Create();
		m_VAO->AddVertexBuffer(m_VertexBuffer);
		m_VAO->SetIndexBuffer(m_IndexBuffer);

		m_Shader = Flibbert::Shader::Create("assets/shaders/DemoBirb/Birb.vert",
		                                    "assets/shaders/DemoBirb/Birb.frag");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", {1, 1, 0, 1});
		m_Shader->BindUniformBlock("Matrices", 0);
	}

	void Birb::OnUpdate(float ts)
	{
		m_Position.y += m_CurrentYSpeed * ts;
		m_CurrentYSpeed = m_CurrentYSpeed + m_FallAccel * ts;
		m_CurrentYSpeed = glm::max(m_CurrentYSpeed, m_MaxFallSpeed);
	}

	void Birb::OnInput(const std::shared_ptr<Flibbert::InputEvent>& event)
	{
		auto keyEvent = dynamic_pointer_cast<Flibbert::InputEventKey>(event);
		if (!keyEvent) return;
		if (keyEvent->IsPressed && keyEvent->Key == Flibbert::Key::Space) {
			FBT_INFO("The birb flops!");
			m_CurrentYSpeed = m_FlopSpeed;
		}
	}
#pragma endregion Birb

#pragma region Pipe
	Pipe::Pipe()
	{
		m_Position = glm::vec2(100, 500);
		m_Size = glm::vec2(50, 150);

		// clang-format off
		float vertices[] = {
			m_Size.x / -2.0f, m_Size.y / -2.0f,
			m_Size.x / 2.0f,  m_Size.y / -2.0f,
			m_Size.x / 2.0f,  m_Size.y / 2.0f,
			m_Size.x / -2.0f, m_Size.y / 2.0f,
		};

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		// clang-format on

		m_VertexBuffer = Flibbert::VertexBuffer::Create(vertices, sizeof(vertices));
		Flibbert::BufferLayout layout = {
		    {Flibbert::ShaderDataType::Float2, "a_Position"},
		};
		m_VertexBuffer->SetLayout(layout);

		m_IndexBuffer =
		    Flibbert::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_VAO = Flibbert::VertexArray::Create();
		m_VAO->AddVertexBuffer(m_VertexBuffer);
		m_VAO->SetIndexBuffer(m_IndexBuffer);

		m_Shader = Flibbert::Shader::Create("assets/shaders/DemoBirb/Pipe.vert",
		                                    "assets/shaders/DemoBirb/Pipe.frag");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", {0, 0.5f, 0.1f, 1});
		m_Shader->BindUniformBlock("Matrices", 0);
	}
#pragma endregion Pipe

#pragma region Scene
	DemoFloppyBirb::DemoFloppyBirb() : m_Renderer(Flibbert::Renderer::Get())
	{
		auto cameraMode = std::make_shared<Flibbert::CameraModeOrthographic>();
		cameraMode->Size = 540.0f;
		cameraMode->NearClip = -1.0f;
		cameraMode->FarClip = 1.0f;
		m_Camera = std::make_unique<Flibbert::Camera>(cameraMode);

		m_CameraBuffer = Flibbert::UniformBuffer::Create(sizeof(Flibbert::CameraBuffer), 0);
	}

	void DemoFloppyBirb::OnUpdate(float ts)
	{
		m_Birb.OnUpdate(ts);
	}

	void DemoFloppyBirb::OnRender()
	{
		Flibbert::CameraBuffer buffer{m_Camera->GetProjectionMatrix(),
		                              m_Camera->GetViewMatrix()};
		m_CameraBuffer->SetData(&buffer, sizeof(Flibbert::CameraBuffer));

		{
			glm::mat4 transform =
			    glm::translate(glm::mat4(1.0f), glm::vec3(m_Pipe.m_Position, 0));
			m_Renderer.Draw(m_Pipe.m_VAO, m_Pipe.m_Shader, transform);
		}

		{
			glm::mat4 transform =
			    glm::translate(glm::mat4(1.0f), glm::vec3(m_Pipe.m_Position.x + 250.0f,
			                                              m_Pipe.m_Position.y, 0));
			m_Renderer.Draw(m_Pipe.m_VAO, m_Pipe.m_Shader, transform);
		}

		// Bird
		{
			glm::mat4 transform =
			    glm::translate(glm::mat4(1.0f), glm::vec3(m_Birb.m_Position, 0));
			m_Renderer.Draw(m_Birb.m_VAO, m_Birb.m_Shader, transform);
		}
	}

	void DemoFloppyBirb::OnImGuiRender()
	{
		ImGui::Text("Floppy Birb!");
		ImGui::Text("Position (%.2f, %.2f)", m_Birb.m_Position.x, m_Birb.m_Position.y);
	}

	void DemoFloppyBirb::OnInput(const std::shared_ptr<Flibbert::InputEvent>& event)
	{
		m_Birb.OnInput(event);
	}
#pragma endregion Scene
} // namespace Demo
