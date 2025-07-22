#pragma once

namespace Flibbert
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniform1i(std::string_view name, int value) = 0;
		virtual void SetUniform1f(std::string_view name, float value) = 0;
		virtual void SetUniform2f(std::string_view name, const glm::vec2& value) = 0;
		virtual void SetUniform4f(std::string_view name, const glm::vec4& value) = 0;
		virtual void SetUniformMat4f(std::string_view name, const glm::mat4& value) = 0;

		virtual void BindUniformBuffer(std::string_view name, uint32_t binding) = 0;

		[[nodiscard]] virtual const std::string& GetName() const { return m_Name; }

		static std::shared_ptr<Shader> Create(std::string_view vertexPath,
		                                      std::string_view fragmentPath);
		static std::shared_ptr<Shader> Create(std::string_view name,
		                                      std::string_view vertexSrc,
		                                      std::string_view fragmentSrc);

	protected:
		std::string m_Name;
	};
} // namespace Flibbert
