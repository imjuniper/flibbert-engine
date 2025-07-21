#include "Flibbert/Renderer/Shader.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/D3D11/D3D11Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Flibbert
{
	std::shared_ptr<Shader> Shader::Create(std::string_view vertexPath,
	                                       std::string_view fragmentPath)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
			case Renderer::API::DirectX11:
				return std::make_shared<D3D11Shader>(vertexPath, fragmentPath);
			default:
				return nullptr;
		}
	}

	std::shared_ptr<Shader> Shader::Create(std::string_view name, std::string_view vertexSrc,
	                                       std::string_view fragmentSrc)
	{
		FBT_CORE_ENSURE_MSG(false, "Cannot create shader with strings yet");
		return nullptr;
	}

} // namespace Flibbert
