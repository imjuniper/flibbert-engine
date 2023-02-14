#pragma once

#include <string>
#include <unordered_map>

class Shader
{
private:
    std::string m_VertexShaderFilePath;
    std::string m_FragmentShaderFilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    // caching for uniforms
public:
    Shader(const std::string &vertexShaderFilepath, const std::string &fragmentShaderFilepath);

    ~Shader();

    void Bind() const;

    void Unbind() const;

    // Set uniforms
    void SetUniform1i(const std::string &name, int value);

    void SetUniform1f(const std::string &name, float value);

    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);

private:
    std::string LoadShader(const std::string &filepath);

    unsigned int CompileShader(unsigned int type, const std::string &source);

    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

    int GetUniformLocation(const std::string &name);
};
