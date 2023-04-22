#include "Demos/DemoFloppyBirb.h"

#include <Flibbert.h>
#include "asset.h"

namespace Demo {

#pragma region Birb
	Birb::Birb() {
		m_Position = glm::vec2(100, 500);
		m_Size = glm::vec2(50, 50);

		// clang-format off
		float positions[] = {
			m_Size.x / -2.0f, m_Size.y / -2.0f, 0.0f, 0.0f,
			m_Size.x / 2.0f,  m_Size.y / -2.0f, 1.0f, 0.0f,
			m_Size.x / 2.0f,  m_Size.y / 2.0f,  1.0f, 1.0f,
			m_Size.x / -2.0f, m_Size.y / 2.0f,  0.0f, 1.0f,
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
		    std::make_unique<Shader>(SHADER_DIR "/DemoBirb/Birb.vert", SHADER_DIR "/DemoBirb/Birb.frag");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 1, 1, 0, 1);
	}

	void Birb::OnUpdate(float deltaTime) {
		m_CurrentYSpeed = m_CurrentYSpeed + m_FallAccel * deltaTime;
		m_CurrentYSpeed = glm::max(m_CurrentYSpeed, m_MaxFallSpeed);

		if (Flibbert::Input::IsKeyDown(Flibbert::KeyCode::Space) && !m_SpacePressed) {
			m_SpacePressed = true;
			FBT_INFO("The birb flops!");
			m_CurrentYSpeed = m_FlopSpeed;
		} else if (Flibbert::Input::IsKeyUp(Flibbert::KeyCode::Space)) {
			m_SpacePressed = false;
		}

		m_Position.y += m_CurrentYSpeed * deltaTime;
	}
#pragma endregion Birb

#pragma region Pipe
	Pipe::Pipe() {
		m_Position = glm::vec2(100, 500);
		m_Size = glm::vec2(50, 150);

		// clang-format off
		float positions[] = {
			m_Size.x / -2.0f, m_Size.y / -2.0f, 0.0f, 0.0f,
			m_Size.x / 2.0f,  m_Size.y / -2.0f, 1.0f, 0.0f,
			m_Size.x / 2.0f,  m_Size.y / 2.0f,  1.0f, 1.0f,
			m_Size.x / -2.0f, m_Size.y / 2.0f,  0.0f, 1.0f,
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
		    std::make_unique<Shader>(SHADER_DIR "/DemoBirb/Pipe.vert", SHADER_DIR "/DemoBirb/Pipe.frag");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0, 0.5f, 0.1f, 1);
	}
#pragma endregion Pipe

#pragma region Scene
	DemoFloppyBirb::DemoFloppyBirb()
	    : m_Projection(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))), m_Birb(), m_Pipe() {
	}

	void DemoFloppyBirb::OnUpdate(float deltaTime) {
		m_Birb.OnUpdate(deltaTime);
	}

	void DemoFloppyBirb::OnRender() {
		Renderer renderer;

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Pipe.m_Position, 0));
			glm::mat4 mvp = m_Projection * m_View * model;
			m_Pipe.m_Shader->Bind();
			m_Pipe.m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_Pipe.m_VAO, *m_Pipe.m_IndexBuffer, *m_Pipe.m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Pipe.m_Position.x + 250.0f, m_Pipe.m_Position.y, 0));
			glm::mat4 mvp = m_Projection * m_View * model;
			m_Pipe.m_Shader->Bind();
			m_Pipe.m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_Pipe.m_VAO, *m_Pipe.m_IndexBuffer, *m_Pipe.m_Shader);
		}

		// Bird
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Birb.m_Position, 0));
			glm::mat4 mvp = m_Projection * m_View * model;
			m_Birb.m_Shader->Bind();
			m_Birb.m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_Birb.m_VAO, *m_Birb.m_IndexBuffer, *m_Birb.m_Shader);
		}
	}

	void DemoFloppyBirb::OnImGuiRender() {
		ImGui::Text("Floppy Birb!");
		ImGui::Text("Position (%.2f, %.2f)", m_Birb.m_Position.x, m_Birb.m_Position.y);
	}
#pragma endregion Scene

} // namespace Demo
