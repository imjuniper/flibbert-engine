#include "Flibbert/Renderer/Shader.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Flibbert
{
	Shader* Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return new OpenGLShader(vertexPath, fragmentPath);
			case Renderer::API::None:
				return nullptr;
		}

		return nullptr;
	}

	Shader* Shader::Create(const std::string& name, const std::string& vertexSrc,
	                       const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				// return new OpenGLShader(name, vertexSrc, fragmentSrc);
				assert(false);
				FBT_CORE_ERROR("Cannot create shader with strings yet");
				return nullptr;
			case Renderer::API::None:
				return nullptr;
		}

		return nullptr;
	}

} // namespace Flibbert
