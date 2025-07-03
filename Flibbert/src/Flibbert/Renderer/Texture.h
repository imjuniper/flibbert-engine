#pragma once

namespace Flibbert
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot) const = 0;
		virtual void Unbind() const = 0;

		[[nodiscard]] uint32_t GetWidth() const { return m_Width; }
		[[nodiscard]] uint32_t GetHeight() const { return m_Height; }
		[[nodiscard]] bool IsLoaded() const { return m_Loaded; }

		static std::shared_ptr<Texture> Create(const std::string& path);

	protected:
		std::string m_Path;
		bool m_Loaded = false;

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_InternalFormat = 0;
		uint32_t m_DataFormat = 0;
	};
} // namespace Flibbert
