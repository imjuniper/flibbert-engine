#include "Platform/OpenGL/OpenGLShader.h"

#include <glad.h>

namespace Flibbert
{
	OpenGLShader::OpenGLShader(std::string_view vertexShaderFilepath,
	                           std::string_view fragmentShaderFilepath)
	    : m_VertexShaderFilePath(vertexShaderFilepath),
	      m_FragmentShaderFilePath(fragmentShaderFilepath), m_RendererID(0)
	{
		ZoneScoped;

		const auto vertexShader = LoadAndPreprocessShader(m_VertexShaderFilePath);
		const auto fragmentShader = LoadAndPreprocessShader(m_FragmentShaderFilePath);

		m_RendererID = CreateShader(vertexShader, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		ZoneScoped;

		glDeleteProgram(m_RendererID);
	}

	uint32_t OpenGLShader::CompileShader(uint32_t type, const std::string& source)
	{
		ZoneScoped;

		uint32_t id = glCreateShader(type);
		// source is not a std::string_view to ensure it's null-terminated
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

			FBT_CORE_ERROR("Failed to compile {} shader:\n\t{}",
			               (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	uint32_t OpenGLShader::CreateShader(const std::string& vertexShader,
	                                    const std::string& fragmentShader)
	{
		ZoneScoped;

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
		ZoneScoped;

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		ZoneScoped;

		glUseProgram(0);
	}

	void OpenGLShader::SetUniform1i(std::string_view name, const int value)
	{
		ZoneScoped;

		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::BindUniformBuffer(std::string_view name, uint32_t binding)
	{
		ZoneScoped;

		uint32_t blockIndex = glGetUniformBlockIndex(m_RendererID, name.data());
		glUniformBlockBinding(m_RendererID, blockIndex, binding);
	}

	int OpenGLShader::GetUniformLocation(std::string_view name)
	{
		ZoneScoped;

		const int location = glGetUniformLocation(m_RendererID, name.data());
		if (location == -1) {
			FBT_CORE_WARN("Uniform {} doesn't exist", name);
		}

		return location;
	}
} // namespace Flibbert
