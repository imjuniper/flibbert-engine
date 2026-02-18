#pragma once

#include <filesystem>

namespace Flibbert {

class IShader
{
public:
	virtual ~IShader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void BindUniformBuffer(std::string_view name, uint32_t binding) = 0;

	[[nodiscard]] virtual const std::string& GetName() const
	{
		return m_Name;
	}

	static std::shared_ptr<IShader> Create(std::string_view vertexPath, std::string_view fragmentPath);
	static std::shared_ptr<IShader> Create(std::string_view name, std::string_view vertexSrc,
	                                      std::string_view fragmentSrc);

	static std::string LoadAndPreprocessShader(const std::filesystem::path& filepath);

protected:
	std::string m_Name;
};

} // namespace Flibbert
