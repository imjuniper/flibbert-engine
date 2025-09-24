#include "Flibbert/Renderer/Renderer.h"

#include "Flibbert/Core/Application.h"

#ifdef FBT_PLATFORM_MACOS
	#include "Platform/Metal/MetalRendererBackend.h"
#endif
#include "Platform/OpenGL/OpenGLRendererBackend.h"
#include "Platform/Vulkan/VulkanRendererBackend.h"

namespace Flibbert
{
	Renderer::API Renderer::s_API = API::Metal;

	Renderer::Renderer()
	{
		switch (s_API) {
			case API::Metal:
#ifdef FBT_PLATFORM_MACOS
				m_Backend = std::make_unique<MetalRendererBackend>();
#else
				FBT_CORE_ENSURE_MSG(false, "Metal is only supported on macOS")
#endif
				break;
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

	void Renderer::ShutdownImGui() const
	{
		ZoneScoped;

		m_Backend->ShutdownImGui();
	}

	void Renderer::BeginFrame() const
	{
		ZoneScoped;

		m_Backend->BeginFrame();
	}

	void Renderer::EndFrame() const
	{
		ZoneScoped;

		m_Backend->EndFrame();
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
