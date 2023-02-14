#include "Shader.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string &vertexShaderFilepath, const std::string &fragmentShaderFilepath)
        : m_VertexShaderFilePath(vertexShaderFilepath), m_FragmentShaderFilePath(fragmentShaderFilepath),
          m_RendererID(0)
{
    std::string vertexShader = LoadShader(vertexShaderFilepath);
    std::string fragmentShader = LoadShader(fragmentShaderFilepath);

    m_RendererID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
    GLCallV(glDeleteProgram(m_RendererID));
}

std::string Shader::LoadShader(const std::string &filepath)
{
    std::ifstream stream(filepath);

    std::stringstream ss;
    ss << stream.rdbuf();

    return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = GLCall(glCreateShader(type));
    const char *src = source.c_str();
    GLCallV(glShaderSource(id, 1, &src, nullptr));
    GLCallV(glCompileShader(id));

    int result;
    GLCallV(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCallV(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char *message = (char *) alloca(length * sizeof(char));
        GLCallV(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader"
                  << std::endl;
        std::cout << message << std::endl;
        GLCallV(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = GLCall(glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCallV(glAttachShader(program, vs));
    GLCallV(glAttachShader(program, fs));
    GLCallV(glLinkProgram(program));
    GLCallV(glValidateProgram(program));

    GLCallV(glDeleteShader(vs));
    GLCallV(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCallV(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCallV(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string &name, int value)
{
    GLCallV(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string &name, float value)
{
    GLCallV(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    GLCallV(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string &name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = GLCall(glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "[Warning] uniform '" << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}
