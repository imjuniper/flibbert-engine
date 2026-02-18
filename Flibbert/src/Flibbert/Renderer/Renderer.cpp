#include "Flibbert/Renderer/Renderer.h"

#include "Flibbert/Core/Application.h"

#include "Backends/OpenGL/OpenGLRendererBackend.h"
#include "Backends/Vulkan/VulkanRendererBackend.h"

namespace Flibbert {

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

glm::vec4 Renderer::GetClearColor() const
{
	FBT_PROFILE_FUNCTION();

	return m_Backend->GetClearColor();
}

void Renderer::SetClearColor(glm::vec4 color) const
{
	FBT_PROFILE_FUNCTION();

	return m_Backend->SetClearColor(color);
}

void Renderer::Clear() const
{
	FBT_PROFILE_FUNCTION();

	return m_Backend->Clear();
}

void Renderer::Submit(const std::shared_ptr<IVertexArray>& vertexArray, const std::shared_ptr<IShader>& shader) const
{
	FBT_PROFILE_FUNCTION();

	return m_Backend->Submit(vertexArray, shader);
}

#if FBT_PROFILING_ENABLED
void Renderer::CaptureProfilerFrameImage()
{
	return m_Backend->CaptureProfilerFrameImage();
}

void Renderer::CollectProfilerGPUTraces()
{
	return m_Backend->CollectProfilerGPUTraces();
}
#endif

Renderer& Renderer::Get()
{
	return Application::Get().GetRenderer();
}

} // namespace Flibbert
