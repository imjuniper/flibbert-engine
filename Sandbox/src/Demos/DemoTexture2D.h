#pragma once

#include "Demos/Demo.h"

#include <Flibbert.h>

namespace Demo {

class DemoTexture2D : public Demo
{
public:
	DemoTexture2D();

	void OnUpdate(float ts) override;
	void OnRender() override;
	void OnImGuiRender() override;
	void OnInput(const std::shared_ptr<Flibbert::InputEvent>& event) override;

private:
	Flibbert::Renderer& m_Renderer;

	std::shared_ptr<Flibbert::IVertexArray> m_VAO;
	std::shared_ptr<Flibbert::IVertexBuffer> m_VertexBuffer;
	std::shared_ptr<Flibbert::IIndexBuffer> m_IndexBuffer;
	std::shared_ptr<Flibbert::IShader> m_Shader;
	std::shared_ptr<Flibbert::ITexture> m_Texture;
	std::shared_ptr<Flibbert::IUniformBuffer> m_PerFrameBuffer;
	std::shared_ptr<Flibbert::IUniformBuffer> m_PerObjectBuffer;
	std::shared_ptr<Flibbert::IUniformBuffer> m_BasicDataBuffer;

	std::unique_ptr<Flibbert::Camera> m_Camera;

	glm::vec3 m_TranslationA, m_TranslationB;
};

} // namespace Demo
