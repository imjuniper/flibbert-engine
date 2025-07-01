#include "Platform/OpenGL/OpenGLShader.h"

#include <glad.h>

#include <fstream>

namespace Flibbert
{
	OpenGLShader::OpenGLShader(const std::string& vertexShaderFilepath,
	                           const std::string& fragmentShaderFilepath)
	    : m_VertexShaderFilePath(vertexShaderFilepath),
	      m_FragmentShaderFilePath(fragmentShaderFilepath), m_RendererID(0)
	{
		std::string vertexShader = LoadShader(vertexShaderFilepath);
		std::string fragmentShader = LoadShader(fragmentShaderFilepath);

		m_RendererID = CreateShader(vertexShader, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::LoadShader(const std::string& filepath)
	{
		std::ifstream stream(filepath);

		std::stringstream ss;
		ss << stream.rdbuf();

		return ss.str();
	}

	uint32_t OpenGLShader::CompileShader(uint32_t type, const std::string& source)
	{
		uint32_t id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);

			std::cout << "Failed to compile "
				  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader"
				  << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	uint32_t OpenGLShader::CreateShader(const std::string& vertexShader,
	                                    const std::string& fragmentShader)
	{
		uint32_t program = glCreateProgram();
		uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniform1i(const std::string& name, const int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniform1f(const std::string& name, const float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniform2f(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::SetUniform4f(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformMat4f(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			std::cout << "[Warning] uniform '" << name << "' doesn't exist!"
				  << std::endl;

		m_UniformLocationCache[name] = location;
		return location;
	}
} // namespace Flibbert
