#pragma once

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

namespace Flibbert
{
	class OpenGLShader
	{
	public:
		OpenGLShader(const std::string& vertexShaderFilepath,
		             const std::string& fragmentShaderFilepath);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;

		// Set uniforms
		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform2f(const std::string& name, float v0, float v1);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, glm::mat4& matrix);

	private:
		std::string LoadShader(const std::string& filepath);
		uint32_t CompileShader(uint32_t type, const std::string& source);
		uint32_t CreateShader(const std::string& vertexShader,
		                      const std::string& fragmentShader);
		int GetUniformLocation(const std::string& name);

	private:
		std::string m_VertexShaderFilePath;
		std::string m_FragmentShaderFilePath;
		uint32_t m_RendererID;
		std::unordered_map<std::string, int> m_UniformLocationCache;
		// caching for uniforms
	};
} // namespace Flibbert
