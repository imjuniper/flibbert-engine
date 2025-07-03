#include "Flibbert/Renderer/Renderer.h"

#include "Flibbert/Core/Application.h"
#include "Platform/OpenGL/OpenGLRendererBackend.h"

namespace Flibbert
{
	Renderer::API Renderer::s_API = API::OpenGL;

	Renderer::Renderer()
	{
		switch (s_API) {
			case API::OpenGL:
				m_Backend = std::make_unique<OpenGLRendererBackend>();
				break;
			default:
				FBT_CORE_CRITICAL("Unsupported or unknown Renderer::API");
				assert(false);
		}
	}

	glm::vec4 Renderer::GetClearColor() const
	{
		return m_Backend->GetClearColor();
	}

	void Renderer::SetClearColor(const glm::vec4& color) const
	{
		return m_Backend->SetClearColor(color);
	}

	void Renderer::Clear() const
	{
		return m_Backend->Clear();
	}

	void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer,
	                    Shader& shader, const glm::mat4& viewProjection,
	                    const glm::mat4& transform) const
	{
		return m_Backend->Draw(vertexArray, indexBuffer, shader, viewProjection, transform);
	}

	Renderer& Renderer::Get()
	{
		return Application::Get().GetRenderer();
	}

	void Renderer::InitGraphicsContext(void* window)
	{
		switch (s_API) {
			case API::OpenGL:
				OpenGLRendererBackend::InitGraphicsContext(
				    static_cast<RGFW_window*>(window));
				break;
			default:
				FBT_CORE_CRITICAL("Unsupported or unknown Renderer::API");
				assert(false);
		}
	}
} // namespace Flibbert
