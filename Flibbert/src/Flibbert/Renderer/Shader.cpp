#include "Flibbert/Renderer/Shader.h"

#include "Flibbert/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <regex>
#include <sstream>

namespace Flibbert
{
	std::shared_ptr<Shader> Shader::Create(std::string_view vertexPath,
	                                       std::string_view fragmentPath)
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
			default:
				return nullptr;
		}
	}

	std::shared_ptr<Shader> Shader::Create(std::string_view name, std::string_view vertexSrc,
	                                       std::string_view fragmentSrc)
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

	// @todo add a "pragma once" for includes. will need to keep the state of what files have been included.
	std::string Shader::LoadAndPreprocessShader(const std::filesystem::path& filepath)
	{
		FBT_PROFILE_FUNCTION();

		std::filesystem::path currentDir = std::filesystem::current_path();
		if (filepath.has_parent_path()) {
			std::filesystem::current_path(filepath.parent_path());
		}
		ON_SCOPE_EXIT
		{
			std::filesystem::current_path(currentDir);
		};

		std::ifstream source(filepath.filename());
		std::stringstream preprocessedShader;

		const std::regex includeRegex("#include \"(.+)\"");

		for (std::string line; getline(source, line);) {
			std::smatch sm;
			if (regex_search(line, sm, includeRegex)) {
				const auto include = sm[1].str();
				if (!FBT_CORE_ENSURE_MSG(std::filesystem::exists(include),
							 "Include shader file {} doesn't exist")) {
					continue;
							 }
				preprocessedShader << LoadAndPreprocessShader(include);
			} else {
				preprocessedShader << line << '\n';
			}
		}
		source.close();

		return preprocessedShader.str();
	}
} // namespace Flibbert
