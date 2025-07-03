#include "Flibbert/Renderer/Shader.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Flibbert
{
	std::shared_ptr<Shader> Shader::Create(const std::string& vertexPath,
	                                       const std::string& fragmentPath)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
			default:
				return nullptr;
		}
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& name,
	                                       const std::string& vertexSrc,
	                                       const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				// return std::make_shared<OpenGLShader>(name, vertexSrc,
				// fragmentSrc);
				FBT_CORE_ENSURE_MSG(false,
				                    "Cannot create OpenGL shader with strings yet");
				return nullptr;
			default:
				return nullptr;
		}
	}

} // namespace Flibbert
