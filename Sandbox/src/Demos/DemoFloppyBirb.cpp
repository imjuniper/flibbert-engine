#include "Demos/DemoFloppyBirb.h"

#include <Flibbert.h>
#include "asset.h"

namespace Demo {

	DemoFloppyBirb::DemoFloppyBirb()
	    : m_Projection(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
	      m_TranslationA(100, 100, 0), m_TranslationB(300, 100, 0) { // clang-format off
		float positions[] = {
			-100.0f, -75.0f, 0.0f, 0.0f,
			100.0f,  -75.0f, 1.0f, 0.0f,
			100.0f,  75.0f,  1.0f, 1.0f,
			-100.0f, 75.0f,  0.0f, 1.0f,
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
		    std::make_unique<Shader>(SHADER_DIR "/Basic.vert", SHADER_DIR "/Basic.frag");
		m_Shader->Bind();
		m_Texture = std::make_unique<Texture>(TEXTURE_DIR "/neko.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	void DemoFloppyBirb::Flop() {
		FBT_INFO("The birb flops!");
	}

	void DemoFloppyBirb::Fall() {
		FBT_INFO("The birb falls :C");
	}

	void DemoFloppyBirb::OnUpdate(float deltaTime) {
		if (Flibbert::Input::IsKeyDown(Flibbert::KeyCode::Space) && !m_SpacePressed) {
			Flop();
			m_SpacePressed = true;
		} else if (Flibbert::Input::IsKeyUp(Flibbert::KeyCode::Space)) {
			m_SpacePressed = false;
		}

		if (!m_SpacePressed) {
			Fall();
		}
	}

	void DemoFloppyBirb::OnRender() {
	}

	void DemoFloppyBirb::OnImGuiRender() {
		ImGui::Text("Floppy Birb!");
	}

} // namespace Demo
