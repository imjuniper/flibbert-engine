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
				FBT_CORE_ENSURE_MSG(false, "Unsupported or unknown Renderer::API");
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
	                    Shader& shader, const glm::mat4& transform) const
	{
		return m_Backend->Draw(vertexArray, indexBuffer, shader, transform);
	}

	Renderer& Renderer::Get()
	{
		return Application::Get().GetRenderer();
	}
} // namespace Flibbert
