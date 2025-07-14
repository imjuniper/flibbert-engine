#pragma once

#include "Demos/Demo.h"

#include <Flibbert.h>

namespace Demo
{
	class DemoCamera3D : public Demo
	{
	public:
		DemoCamera3D();

		void OnUpdate(float ts) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnInput(const std::shared_ptr<Flibbert::InputEvent>& event) override;

	private:
		Flibbert::Renderer& m_Renderer;

		std::shared_ptr<Flibbert::VertexArray> m_VAO;
		std::shared_ptr<Flibbert::VertexBuffer> m_VertexBuffer;
		std::shared_ptr<Flibbert::IndexBuffer> m_IndexBuffer;
		std::shared_ptr<Flibbert::Shader> m_Shader;
		std::shared_ptr<Flibbert::Texture> m_Texture;
		std::shared_ptr<Flibbert::UniformBuffer> m_CameraBuffer;

		std::unique_ptr<Flibbert::Camera> m_Camera;

		std::shared_ptr<Flibbert::CameraModePerspective> m_CameraModePerspective;
		std::shared_ptr<Flibbert::CameraModeOrthographic> m_CameraModeOrthographic;

		bool m_UsingOrthographicMode = false;

		glm::vec3 m_TranslationA, m_TranslationB;
	};
} // namespace Demo
