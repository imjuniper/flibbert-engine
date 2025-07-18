#include "Flibbert/Renderer/Renderer.h"

#include "Flibbert/Core/Application.h"
#include "Platform/D3D11/D3D11RendererBackend.h"
#include "Platform/OpenGL/OpenGLRendererBackend.h"

namespace Flibbert
{
	Renderer::API Renderer::s_API = API::DirectX11;

	Renderer::Renderer()
	{
		switch (s_API) {
			case API::OpenGL:
				m_Backend = std::make_unique<OpenGLRendererBackend>();
				break;
			case API::DirectX11:
				m_Backend = std::make_unique<D3D11RendererBackend>();
				FBT_CORE_WARN("The D3D11 Renderer Backend is very WIP.");
				break;
			default:
				FBT_CORE_ENSURE_MSG(false, "Unsupported or unknown Renderer::API");
		}
	}

	void Renderer::InitImGui() const
	{
		m_Backend->InitImGui();
	}

	void Renderer::BeginImGuiFrame() const
	{
		m_Backend->BeginImGuiFrame();
	}

	void Renderer::EndImGuiFrame() const
	{
		m_Backend->EndImGuiFrame();
	}

	void Renderer::ShutdownImGui() const
	{
		m_Backend->ShutdownImGui();
	}

	glm::vec4 Renderer::GetClearColor() const
	{
		return m_Backend->GetClearColor();
	}

	void Renderer::SetClearColor(glm::vec4 color) const
	{
		return m_Backend->SetClearColor(color);
	}

	void Renderer::Clear() const
	{
		return m_Backend->Clear();
	}

	void Renderer::Draw(const std::shared_ptr<VertexArray>& vertexArray,
	                    const std::shared_ptr<Shader>& shader, glm::mat4 transform) const
	{
		return m_Backend->Draw(vertexArray, shader, transform);
	}

	void Renderer::Present() const
	{
		return m_Backend->Present();
	}

	Renderer& Renderer::Get()
	{
		return Application::Get().GetRenderer();
	}
} // namespace Flibbert
