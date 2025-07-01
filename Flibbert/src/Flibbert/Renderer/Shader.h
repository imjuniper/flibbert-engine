#pragma once

namespace Flibbert
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniform1i(const std::string& name, int value) = 0;
		virtual void SetUniform1f(const std::string& name, float value) = 0;
		virtual void SetUniform2f(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetUniform4f(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& value) = 0;

		[[nodiscard]] virtual const std::string& GetName() const { return m_Name; }

		static Shader* Create(const std::string& vertexPath,
		                      const std::string& fragmentPath);
		static Shader* Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

	protected:
		std::string m_Name;
	};
} // namespace Flibbert
