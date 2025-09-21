#include "Flibbert/Renderer/Renderer.h"

#include "Flibbert/Core/Application.h"

#include "Platform/OpenGL/OpenGLRendererBackend.h"
#include "Platform/Vulkan/VulkanRendererBackend.h"

namespace Flibbert
{
	Renderer::API Renderer::s_API = API::OpenGL;

	Renderer::Renderer()
	{
		switch (s_API) {
			case API::OpenGL:
				m_Backend = std::make_unique<OpenGLRendererBackend>();
				break;
			case API::Vulkan:
				m_Backend = std::make_unique<VulkanRendererBackend>();
				break;
			default:
				FBT_CORE_ENSURE_MSG(false, "Unsupported or unknown Renderer::API");
		}
	}

	void Renderer::InitImGui() const
	{
		ZoneScoped;

		m_Backend->InitImGui();
	}

	void Renderer::BeginImGuiFrame() const
	{
		ZoneScoped;

		m_Backend->BeginImGuiFrame();
	}

	void Renderer::EndImGuiFrame() const
	{
		ZoneScoped;

		m_Backend->EndImGuiFrame();
	}

	void Renderer::ShutdownImGui() const
	{
		ZoneScoped;

		m_Backend->ShutdownImGui();
	}

	glm::vec4 Renderer::GetClearColor() const
	{
		ZoneScoped;

		return m_Backend->GetClearColor();
	}

	void Renderer::SetClearColor(glm::vec4 color) const
	{
		ZoneScoped;

		return m_Backend->SetClearColor(color);
	}

	void Renderer::Clear() const
	{
		ZoneScoped;

		return m_Backend->Clear();
	}

	void Renderer::Draw(const std::shared_ptr<VertexArray>& vertexArray,
	                    const std::shared_ptr<Shader>& shader) const
	{
		ZoneScoped;

		return m_Backend->Draw(vertexArray, shader);
	}

#if FBT_PROFILING_ENABLED
	void Renderer::CaptureTracyFrameImage()
	{
		return m_Backend->CaptureTracyFrameImage();
	}

	void Renderer::CollectTracyGPUTraces()
	{
		return m_Backend->CollectTracyGPUTraces();
	}
#endif

	Renderer& Renderer::Get()
	{
		return Application::Get().GetRenderer();
	}
} // namespace Flibbert
