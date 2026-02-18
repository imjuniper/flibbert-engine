#include "Backends/OpenGL/OpenGLShader.h"

#include <glad.h>

namespace Flibbert {

OpenGLShader::OpenGLShader(std::string_view vertexShaderFilepath, std::string_view fragmentShaderFilepath)
    : m_VertexShaderFilePath(vertexShaderFilepath), m_FragmentShaderFilePath(fragmentShaderFilepath), m_RendererID(0)
{
	FBT_PROFILE_FUNCTION();

	const auto vertexShader = LoadAndPreprocessShader(m_VertexShaderFilePath);
	const auto fragmentShader = LoadAndPreprocessShader(m_FragmentShaderFilePath);

	m_RendererID = CreateShader(vertexShader, fragmentShader);
	if (m_RendererID != 0) {
		CacheUniformLocations();
	}
}

OpenGLShader::~OpenGLShader()
{
	FBT_PROFILE_FUNCTION();

	glDeleteProgram(m_RendererID);
}

uint32_t OpenGLShader::CompileShader(uint32_t type, const std::string& source)
{
	FBT_PROFILE_FUNCTION();

	// @todo migrate to SPIR-V here, so code can be reused for Vulkan and maybe DXIL? aka HLSL -> SPIR-V &
	// DXIL

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

		FBT_CORE_ERROR("Failed to compile {} shader:\n\t{}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"),
		               message);
		glDeleteShader(id);
		return 0;
	}

	return id;
}

uint32_t OpenGLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	FBT_PROFILE_FUNCTION();

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
	FBT_PROFILE_FUNCTION();

	glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const
{
	FBT_PROFILE_FUNCTION();

	glUseProgram(0);
}

void OpenGLShader::BindUniformBuffer(std::string_view name, uint32_t binding)
{
	FBT_PROFILE_FUNCTION();

	uint32_t blockIndex = glGetUniformBlockIndex(m_RendererID, name.data());
	glUniformBlockBinding(m_RendererID, blockIndex, binding);
}

void OpenGLShader::CacheUniformLocations()
{
	int32_t uniformCount = 0;
	glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &uniformCount);

	if (uniformCount != 0) {
		GLint maxNameLength = 0;
		GLsizei length = 0;
		GLsizei size = 0;
		GLenum type = GL_NONE;
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

		auto uniformName = std::make_unique<char[]>(maxNameLength);

		for (GLint i = 0; i < uniformCount; ++i) {
			glGetActiveUniform(m_RendererID, i, maxNameLength, &length, &size, &type, uniformName.get());

			OpenGLUniformInfo uniformInfo = {};
			uniformInfo.Location = glGetUniformLocation(m_RendererID, uniformName.get());
			uniformInfo.Size = size;

			m_Uniforms.emplace(std::make_pair(std::string(uniformName.get(), length), uniformInfo));
		}
	}
}

} // namespace Flibbert
